# HDFS简要介绍

## 1. 简介及历史
Hadoop分布式文件系统(HDFS)被设计成适合运行在通用硬件(commodity hardware)上的分布式文件系统。它和现有的分布式文件系统有很多共同点。但同时，它和其他的分布式文件系统的区别也是很明显的。HDFS是一个高度容错性的系统，适合部署在廉价的机器上。HDFS能提供高吞吐量的数据访问，非常适合大规模数据集上的应用。HDFS放宽了一部分POSIX约束，来实现流式读取文件系统数据的目的。HDFS在最开始是作为Apache Nutch搜索引擎项目的基础架构而开发的。HDFS是Apache Hadoop Core项目的一部分。

 * Google 2003 GFS: ACM SOSP (Symposium on Operating Systems Principles)
 * Google 2004 MapReduce: OSDI (Operating Systems Design and Implementation)
 * Doug Cutting: NDFS 2004
 * Doug Cutting: Nutch MapReduce 2005
 * Nutch 0.8.0 完全构建在Hadoop之上，2006年2月，Doug Cutting加入Yahoo。
 * 2008年1月，Hadoop成为Apache顶级项目。(Facebook, 纽约时报在亚马逊EC2云计算上运用100台机器，4TB扫描文件压缩转换成PDF用时不到24小时)
 * 2008年2月，Yahoo采用10000多个核的linux集群运行hadoop索引网页。
 * 2008年 --> 爆发式发展: HBase, ZooKeeper, Mahout, 2009年的Pig, Hive等
专注hadoop的公司： Cloudera, Hortonworks

## 2. 同类产品对比
### 2.1 GFS
google的三大基石 gfs,bigtable,map-reduce 相对应的开源产品 gfs:kfs(据传google创史人的同窗所创)，hdfs(hadoop的子项目) bigtable:hbase(hadoop的子项目)，Hypertable（从hbase项目组分离出去的，用c++实现） map-reduce:hadoop（apache的项目，java实现，目前创史人在yahoo全力打造，已有2000个以上的节点并行计算的规模）。

HDFS基本可以认为是GFS的一个简化版实现，二者因此有很多相似之处。

 * 首先，GFS和HDFS都采用单一主控机+多台工作机的模式，由一台主控机(Master)存储系统全部元数据，并实现数据的分布、复制、备份决策，主控机还实现了元数据的checkpoint和操作日志记录及回放功能。
 * 其次，GFS和HDFS都通过数据分块和复制（多副本，一般是3）来提供更高的可靠性和更高的性能。当其中一个副本不可用时，系统都提供副本自动复制功能。同时，针对数据读多于写的特点，读服务被分配到多个副本所在机器，提供了系统的整体性能。
 * 最后，GFS和HDFS都提供了一个树结构的文件系统，实现了类似与Linux下的文件复制、改名、移动、创建、删除操作以及简单的权限管理等。

然而，GFS和HDFS在关键点的设计上差异很大，HDFS为了规避GFS的复杂度进行了很多简化。

 * 首先，GFS最为复杂的部分是对多客户端并发追加同一个文件，即多客户端并发Append模型。GFS允许文件被多次或者多个客户端同时打开以追加数据，以记录为单位。假设GFS追加记录的大小为16KB ~ 16MB之间，平均大小为1MB，如果每次追加都访问GFS Master显然很低效，因此，GFS通过Lease机制将每个Chunk的写权限授权给Chunk Server。写Lease的含义是Chunk Server对某个Chunk在Lease有效期内(假设为12s)有写权限，拥有Lease的Chunk Server称为Primary Chunk Server，如果Primary Chunk Server宕机，Lease有效期过后Chunk的写Lease可以分配给其它Chunk Server。多客户端并发追加同一个文件导致Chunk Server需要对记录进行定序，客户端的写操作失败后可能重试，从而产生重复记录，再加上客户端API为异步模型，又产生了记录乱序问题。Append模型下重复记录、乱序等问题加上Lease机制，尤其是同一个Chunk的Lease可能在Chunk Server之间迁移，极大地提高了系统设计和一致性模型的复杂度。而在HDFS中，HDFS文件只允许一次打开并追加数据，客户端先把所有数据写入本地的临时文件中，等到数据量达到一个Chunk的大小（通常为64MB），请求HDFS Master分配工作机及Chunk编号，将一个Chunk的数据一次性写入HDFS文件。由于累积64MB数据才进行实际写HDFS系统，对HDFS Master造成的压力不大，不需要类似GFS中的将写Lease授权给工作机的机制，且没有了重复记录和乱序的问题，大大地简化了系统的设计。然而，我们必须知道，HDFS由于不支持Append模型带来的很多问题，构建于HDFS之上的Hypertable和HBase需要使用HDFS存放表格系统的操作日志，由于HDFS的客户端需要攒到64MB数据才一次性写入到HDFS中，Hypertable和HBase中的表格服务节点(对应于Bigtable中的Tablet Server)如果宕机，部分操作日志没有写入到HDFS，可能会丢数据。
 * 其次是Master单点失效的处理。GFS中采用主从模式备份Master的系统元数据，当主Master失效时，可以通过分布式选举备机接替主Master继续对外提供服务，而由于Replication及主备切换本身有一定的复杂性，HDFS Master的持久化数据只写入到本机（可能写入多份存放到Master机器的多个磁盘中防止某个磁盘损害），出现故障时需要人工介入。另外一点是对快照的支持。GFS通过内部采用copy-on-write的数据结构实现集群快照功能，而HDFS不提供快照功能。在大规模分布式系统中，程序有bug是很正常的情况，虽然大多数情况下可以修复bug，不过很难通过补偿操作将系统数据恢复到一致的状态，往往需要底层系统提供快照功能，将系统恢复到最近的某个一致状态。
 * 总之，HDFS基本可以认为是GFS的简化版，由于时间及应用场景等各方面的原因对GFS的功能做了一定的简化，大大降低了复杂度。

### 2.2 KFS
Google两个共同创始人的两个大学同窗（印度人）Anand Rajaraman和Venky Harinarayan，创立的一个新的搜索引擎Kosmix并于2012年捐献了一个克隆GFS的文件系统KFS项目。Hadoop和Hypertable这两个项目也开始支持KFS来做底层的存储。KFS是用C++写的，但是其client支持C++，Java和Python。
 * 支持同一文件多次写入和Append，不像HDFS支持一次写入多次读取和不支持Append（最近要增加Append，但是遇到许多问题）。
 * 文件及时有效，当应用程序创建一个文件时,文件名在系统马上有效。不像HDFS文件只当输入流关闭时才在系统中有效，因此，如果应用程序在关闭前出现异常导致没有关闭输入流，数据将会丢失。

## 3. 优缺点
### 3.1 优点
* __高可靠性__。Hadoop按位存储和处理数据的能力值得人们信赖;
* __高扩展性__。Hadoop是在可用的计算机集簇间分配数据并完成计算任务的，这些集簇可以方便地扩展到数以千计的节点中。
* __高效性__。Hadoop能够在节点之间动态地移动数据，并保证各个节点的动态平衡，因此处理速度非常快。
* __高容错性__。Hadoop能够自动保存数据的多个副本，并且能够自动将失败的任务重新分配。

### 3.2 缺点
* __低时间延迟的数据访问__。例如几十毫秒范围，不适合在HDFS上运行。HDFS是为高数据吞吐量应用优化，会以高时间延迟为代价。目前对于低延迟的访问需求，HBase是一种更好的选择。
* __大量的小文件__。由于namenode将文件系统的元数据存储在内存中，因此该文件系统所能存储的文件总数受限于namenode的内存容量。
* __多用户写入，任意修改文件__。HDFS中的文件只能有一个writer，且写操作总是将数据添加在文件的末尾，它不支持具有多个写入者的操作，也不支持在文件的任意位置进行修改。

## 4. 结构
### 4.1 相关概念
#### 4.1.1 数据块(block)
* HDFS(Hadoop Distributed File System)默认的最基本的存储单位是64M的数据块。
* 和普通文件系统相同的是，HDFS中的文件是被分成64M一块的数据块存储的。
* 不同于普通文件系统的是，HDFS中，如果一个文件小于一个数据块的大小，并不占用整个数据块存储空间。
#### 4.1.2 三种结点
 * 元数据节点用来管理文件系统的命名空间
 > 其将所有的文件和文件夹的元数据保存在一个文件系统树中。
 > 这些信息也会在硬盘上保存成以下文件：命名空间镜像(namespace image)及修改日志(edit log)
 > 其还保存了一个文件包括哪些数据块，分布在哪些数据节点上。然而这些信息并不存储在硬盘上，而是在系统启动的时候从数据节点收集而成的。

 * 数据节点是文件系统中真正存储数据的地方。
 > 客户端(client)或者元数据信息(namenode)可以向数据节点请求写入或者读出数据块。
 > 其周期性的向元数据节点回报其存储的数据块信息。

 * 从元数据节点(secondary namenode)
 > 从元数据节点并不是元数据节点出现问题时候的备用节点，它和元数据节点负责不同的事情。
 > 其主要功能就是周期性将元数据节点的命名空间镜像文件和修改日志合并，以防日志文件过大。这点在下面会相信叙述。
 > 合并过后的命名空间镜像文件也在从元数据节点保存了一份，以防元数据节点失败的时候，可以恢复。

#### 4.1.3 文件系统命名空间映像文件及修改日志
 * 当文件系统客户端(client)进行写操作时，首先把它记录在修改日志中(edit log)
 * 元数据节点在内存中保存了文件系统的元数据信息。在记录了修改日志后，元数据节点则修改内存中的数据结构。
 * 每次的写操作成功之前，修改日志都会同步(sync)到文件系统。
 * fsimage文件，也即命名空间映像文件，是内存中的元数据在硬盘上的checkpoint，它是一种序列化的格式，并不能够在硬盘上直接修改。
 * 同数据的机制相似，当元数据节点失败时，则最新checkpoint的元数据信息从fsimage加载到内存中，然后逐一重新执行修改日志中的操作。
 * 从元数据节点就是用来帮助元数据节点将内存中的元数据信息checkpoint到硬盘上的
 * checkpoint的过程如下：
 >  * 从元数据节点通知元数据节点生成新的日志文件，以后的日志都写到新的日志文件中。
 >  * 从元数据节点用http get从元数据节点获得fsimage文件及旧的日志文件。
 >  * 从元数据节点将fsimage文件加载到内存中，并执行日志文件中的操作，然后生成新的fsimage文件。
 >  * 从元数据节点奖新的fsimage文件用http post传回元数据节点
 >  * 元数据节点可以将旧的fsimage文件及旧的日志文件，换为新的fsimage文件和新的日志文件(第一步生成的)，然后更新fstime文件，写入此次checkpoint的时间。
 >  * 这样元数据节点中的fsimage文件保存了最新的checkpoint的元数据信息，日志文件也重新开始，不会变的很大了。
![enter image description here][1]

Namenode节点文件夹结构
![enter image description here][2]
Secondary Namenode节点文件夹结构
![enter image description here][3]
Datanode节点文件夹结构
![enter image description here][4]

### 4.2 Namenode和Datanode
HDFS 采用 master/slave 架构。一个 HDFS 集群是有一个 Namenode 和一定数目的Datanode 组成。Namenode 是一个中心服务器，负责管理文件系统的 namespace 和客户端对文件的访问。Datanode 在集群中一般是一个节点一个，负责管理节点上它们附带的存储。在内部，一个文件其实分成一个或多个 block，这些 block 存储在 Datanode 集合里。Namenode 执行文件系统的 namespace 操作，例如打开、关闭、重命名文件和目录，同时决定 block 到具体 Datanode 节点的映射。Datanode 在 Namenode 的指挥下进行 block 的创建、删除和复制。Namenode 和 Datanode 都是设计成可以跑在普通的廉价的运行 linux 的机器上。HDFS 采用 java 语言开发，因此可以部署在很大范围的机器上。一个典型的部署场景是一台机器跑一个单独的 Namenode 节点，集群中的其他机器各跑一个 Datanode 实例。这个架构并不排除一台机器上跑多个 Datanode，不过这比较少见。
![enter image description here][5]

### 4.3 数据读流程
 * 客户端(client)用FileSystem的open()函数打开文件
 * DistributedFileSystem用RPC调用元数据节点，得到文件的数据块信息。
 * 对于每一个数据块，元数据节点返回保存数据块的数据节点的地址。
 * DistributedFileSystem返回FSDataInputStream给客户端，用来读取数据。
 * 客户端调用stream的read()函数开始读取数据。
 * DFSInputStream连接保存此文件第一个数据块的最近的数据节点。
 * Data从数据节点读到客户端(client)
 * 当此数据块读取完毕时，DFSInputStream关闭和此数据节点的连接，然后连接此文件下一个数据块的最近的数据节点。
 * 当客户端读取完毕数据的时候，调用FSDataInputStream的close函数。
 * 在读取数据的过程中，如果客户端在与数据节点通信出现错误，则尝试连接包含此数据块的下一个数据节点。
 * 失败的数据节点将被记录，以后不再连接。
![enter image description here][6]

### 4.4 数据写入流程
 * 客户端调用create()来创建文件
 * DistributedFileSystem用RPC调用元数据节点，在文件系统的命名空间中创建一个新的文件。
 * 元数据节点首先确定文件原来不存在，并且客户端有创建文件的权限，然后创建新文件。
 * DistributedFileSystem返回DFSOutputStream，客户端用于写数据。
 * 客户端开始写入数据，DFSOutputStream将数据分成块，写入data queue。
 * Data queue由Data Streamer读取，并通知元数据节点分配数据节点，用来存储数据块(每块默认复制3块)。分配的数据节点放在一个pipeline里。
 * Data Streamer将数据块写入pipeline中的第一个数据节点。第一个数据节点将数据块发送给第二个数据节点。第二个数据节点将数据发送给第三个数据节点。
 * DFSOutputStream为发出去的数据块保存了ack queue，等待pipeline中的数据节点告知数据已经写入成功。
 * 如果数据节点在写入的过程中失败：
 * 关闭pipeline，将ack queue中的数据块放入data queue的开始。
 * 当前的数据块在已经写入的数据节点中被元数据节点赋予新的标示，则错误节点重启后能够察觉其数据块是过时的，会被删除。
 * 失败的数据节点从pipeline中移除，另外的数据块则写入pipeline中的另外两个数据节点。
 * 元数据节点则被通知此数据块是复制块数不足，将来会再创建第三份备份。
 * 当客户端结束写入数据，则调用stream的close函数。此操作将所有的数据块写入pipeline中的数据节点，并等待ack queue返回成功。最后通知元数据节点写入完毕。
![enter image description here][7]

## 5. 使用
### 5.1 FS shell
调用文件系统(FS)Shell命令应使用 bin/hadoop fs <args> 的形式。 所有的的FS shell命令使用URI路径作为参数。URI格式是scheme://authority/path 。对 HDFS文件系统，scheme是hdfs ，对本地文件系统，scheme是file 。其中scheme和 authority参数都是可选的，如果未加指定，就会使用配置中指定的默认scheme。一个HDFS文件或目录比如/parent/child 可 以表示成hdfs://namenode:namenodeport/parent/child ，或者更简单的/parent/child （假 设你配置文件中的默认值是namenode:namenodeport ）。大多数FS Shell命令的行为和对应的Unix Shell命令类似，不同之处会在下面介绍各命令使用详情时指出。出错信息会输出到stderr ，其他信息输出到stdout 。

__cat__
使用方法：hadoop fs -cat URI [URI …]
将路径指定文件的内容输出到stdout。
示例：
 > hadoop fs -cat hdfs://host1:port1/file1 hdfs://host2:port2/file2
 > hadoop fs -cat file:///file3 /user/hadoop/file4
返回值：
 > 成功返回0，失败返回-1。

__chgrp__
使用方法：
 > hadoop fs -chgrp [-R] GROUP URI [URI …] 
改变文件所属的组。使用-R 将使改变在目录结构下递归进行。命令的使用者必须是 文件的所有者或者超级用户。更多的信息请参见HDFS 权限用户指南。

__chmod__
使用方法：
 > hadoop fs -chmod [-R] <MODE[,MODE]... | OCTALMODE> URI [URI …]
改变文件的权限。使用-R 将使改变在目录结构下递归进行。命令的使用者必须是文 件的所有者或者超级用户。更多的信息请参见HDFS 权限用户指南。

__chown__
使用方法：
 > hadoop fs -chown [-R] [OWNER][:[GROUP]] URI [URI ]
改变文件的拥有者。使用-R 将使改变在目录结构下递归进行。命令的使用者必须是 超级用户。更多的信息请参见HDFS 权限用户指南。

__copyFromLocal__
使用方法：
 > hadoop fs -copyFromLocal <localsrc> URI
除了限定源路径是一个本地文件外，和put 命令相似。

__copyToLocal__
使用方法：
 > hadoop fs -copyToLocal [-ignorecrc] [-crc] URI <localdst>
除了限定目标路径是一个本地文件外，和get 命令类似。

__cp__
使用方法：hadoop fs -cp URI [URI …] <dest>
将文件从源路径复制到目标路径。这个命令允许有多个源路径，此时目标路径必须是一个目录。
示例：
 > hadoop fs -cp /user/hadoop/file1 /user/hadoop/file2
 > hadoop fs -cp /user/hadoop/file1 /user/hadoop/file2 /user/hadoop/dir
返回值：
 > 成功返回0，失败返回-1。

__du__
使用方法：hadoop fs -du URI [URI …]
显示目录中所有文件的大小，或者当只指定一个文件时，显示此文件的大小。
示例：
 > hadoop fs -du /user/hadoop/dir1 /user/hadoop/file1 hdfs://host:port/user/hadoop/dir1
返回值：
 > 成功返回0，失败返回-1。

__dus__
使用方法：
 > hadoop fs -dus <args>
显示文件的大小。

__expunge__
使用方法：
 > hadoop fs -expunge
清空回收站。

__get__
使用方法：
 > hadoop fs -get [-ignorecrc] [-crc] <src> <localdst>
复制文件到本地文件系统。可用-ignorecrc 选项复制CRC校验失败的文 件。使用-crc 选项复制文件以及CRC信息。
示例：
 > hadoop fs -get /user/hadoop/file localfile
 > hadoop fs -get hdfs://host:port/user/hadoop/file localfile
返回值：
 > 成功返回0，失败返回-1。

__getmerge__
使用方法：
 > hadoop fs -getmerge <src> <localdst> [addnl]
接受一个源目录和一个目标文件作为输入，并且将源目录中所有的文件连接成本地目标文件。addnl 是可选的，用于指定在每个文件结尾添加一个换行符。

__ls__
使用方法：
 > hadoop fs -ls <args>
如果是文件，则按照如下格式返回文件信息：
文件名 <副本数> 文件大小 修改日期 修改时间 权限 用户ID 组ID
如果是目录，则返回它直接子文件的一个列表，就像在Unix中一样。目录返回列表的信息如下：
目录名 <dir> 修改日期 修改时间 权限 用户ID 组ID
示例：
 > hadoop fs -ls /user/hadoop/file1 /user/hadoop/file2 hdfs://host:port/user/hadoop/dir1 /nonexistentfile
返回值：
 > 成功返回0，失败返回-1。

__lsr__
使用方法：
 > hadoop fs -lsr <args>
ls 命令的递归版本。类似于Unix中的ls -R。

__mkdir__
使用方法：
 > hadoop fs -mkdir <paths>
接受路径指定的uri作为参数，创建这些目录。其行为类似于Unix的mkdir -p，它会创建路径中的各级父目录。
示例：
 > hadoop fs -mkdir /user/hadoop/dir1 /user/hadoop/dir2
 > hadoop fs -mkdir hdfs://host1:port1/user/hadoop/dir hdfs://host2:port2/user/hadoop/dir
返回值：
 > 成功返回0，失败返回-1。

__movefromLocal__
使用方法：
 > dfs -moveFromLocal <src> <dst>
输出一个”not implemented“信息。

__mv__
使用方法：
 > hadoop fs -mv URI [URI …] <dest>
将文件从源路径移动到目标路径。这个命令允许有多个源路径，此时目标路径必须是一个目录。不允许在不同的文件系统间移动文件。
示例：
 > hadoop fs -mv /user/hadoop/file1 /user/hadoop/file2
 > hadoop fs -mv hdfs://host:port/file1 hdfs://host:port/file2 hdfs://host:port/file3 hdfs://host:port/dir1
返回值：
 > 成功返回0，失败返回-1。

__put__
使用方法：
 > hadoop fs -put <localsrc> ... <dst>
从本地文件系统中复制单个或多个源路径到目标文件系统。也支持从标准输入中读取输入写入目标文件系统。
 > hadoop fs -put localfile /user/hadoop/hadoopfile
 > hadoop fs -put localfile1 localfile2 /user/hadoop/hadoopdir
 > hadoop fs -put localfile hdfs://host:port/hadoop/hadoopfile
 > hadoop fs -put - hdfs://host:port/hadoop/hadoopfile
从标准输入中读取输入。
返回值：
 > 成功返回0，失败返回-1。

__rm__
使用方法：
 > hadoop fs -rm URI [URI …]
删除指定的文件。只删除非空目录和文件。请参考rmr命令了解递归删除。
示例：
 > hadoop fs -rm hdfs://host:port/file /user/hadoop/emptydir
返回值：
 > 成功返回0，失败返回-1。

__rmr__
使用方法：
 > hadoop fs -rmr URI [URI …]
delete的递归版本。
示例：
 > hadoop fs -rmr /user/hadoop/dir
 > hadoop fs -rmr hdfs://host:port/user/hadoop/dir
返回值：
 > 成功返回0，失败返回-1。

__setrep__
使用方法：
 > hadoop fs -setrep [-R] <path>
改变一个文件的副本系数。-R选项用于递归改变目录下所有文件的副本系数。
示例：
 > hadoop fs -setrep -w 3 -R /user/hadoop/dir1
返回值：
 > 成功返回0，失败返回-1。

__stat__
使用方法：
 > hadoop fs -stat URI [URI …]
返回指定路径的统计信息。
示例：
 > hadoop fs -stat path
返回值：
 > 成功返回0，失败返回-1。

__tail__
使用方法：
 > hadoop fs -tail [-f] URI
将文件尾部1K字节的内容输出到stdout。支持-f选项，行为和Unix中一致。
示例：
 > hadoop fs -tail pathname
返回值：
 > 成功返回0，失败返回-1。

__test__
使用方法：
 > hadoop fs -test -[ezd] URI
选项：
-e 检查文件是否存在。如果存在则返回0。
-z 检查文件是否是0字节。如果是则返回0。
-d 如果路径是个目录，则返回1，否则返回0。
示例：
 > hadoop fs -test -e filename

__text__
使用方法：
 > hadoop fs -text <src>
将源文件输出为文本格式。允许的格式是zip和TextRecordInputStream。

__touchz__
使用方法：hadoop fs -touchz URI [URI …]
创建一个0字节的空文件。
示例：
 > hadoop -touchz pathname

## 6. 常见问题
### 6.1 文件删除与恢复
当删除文件时，如果hadoop配置了fs.trash.interval设置成非0值，则文件会移到/trash或./.Trash目录下，而不是直接删除，然后会保留一段时间后删除，这个时长就是fs.trash.interval设置的值，单位为分钟。trash目录下保留和所删除的原目一样的结构，但只保留了最后一份删除，没有保留多份删除记录。



## 参考资料
1. [HDFS Architecture Guide](http://hadoop.apache.org/docs/r1.2.1/hdfs_design.html)
2. [《Hadoop技术内幕 深入解析HADOOP COMMON和HDFS架构设计与实现原理》](http://pan.baidu.com/s/1ntiaNWX)（提取密码：t9vw）
3. [Hadoop Shell Commands](http://hadoop.apache.org/docs/r0.18.3/hdfs_shell.html)
4. 《hadoop权威指南》
5. [GFS, HDFS, Blob File System架构对比](http://blog.csdn.net/adcxf/article/details/6284871)


  [1]: http://images.cnblogs.com/cnblogs_com/forfuture1978/WindowsLiveWriter/Hadoop_10C71/hadoop01_thumb.jpg
  [2]: http://images.cnblogs.com/cnblogs_com/forfuture1978/WindowsLiveWriter/Hadoop_10C71/image_thumb.png
  [3]: http://images.cnblogs.com/cnblogs_com/forfuture1978/WindowsLiveWriter/Hadoop_10C71/image_thumb_1.png
  [4]: http://images.cnblogs.com/cnblogs_com/forfuture1978/WindowsLiveWriter/Hadoop_10C71/image_thumb_2.png
  [5]: http://hadoop.apache.org/docs/r1.2.1/images/hdfsarchitecture.gif
  [6]: http://images.cnblogs.com/cnblogs_com/forfuture1978/WindowsLiveWriter/Hadoop_10C71/image_thumb_3.png
  [7]: http://images.cnblogs.com/cnblogs_com/forfuture1978/WindowsLiveWriter/Hadoop_10C71/image_thumb_4.png