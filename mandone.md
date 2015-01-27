
mandone internal API (手动打done API)
============================


<!-- 这里以部署机器的 host: hz01-cm-bsidx31.hz01.baidu.com 为例 -->


### mandone查询打done目录名称API

    http://hz01-cm-bsidx31.hz01.baidu.com:8482/mandone_list

* 参数:  (除callback外，均为必填参数)

    callback: 伪json调用方式的js回调函数名，用于跨域访问

* 示例: 

    http://hz01-cm-bsidx31.hz01.baidu.com:8482/mandone_list

* 返回结果: 

    ```
    {  
       "data":{  
          "bdir":[  
             "/app/bigpipe_log/dayu_beidou_search_normal_pb",
             "/app/bigpipe_log/dayu_beidou_search_spam_pb",
             "/app/bigpipe_log/dayu_dsp_normal_pb",
             "/app/bigpipe_log/dayu_dsp_spam_pb",
             "/app/bigpipe_log/dayu_lu_search_normal_pb",
             "/app/bigpipe_log/dayu_lu_search_spam_pb",
             "/app/bigpipe_log/dayu_newdsp_clickcpm_normal_txt",
             "/app/bigpipe_log/dayu_newdsp_clickcpm_spam_txt",
             "/app/bigpipe_log/dayu_newdsp_normal_pb",
             "/app/bigpipe_log/dayu_newdsp_normal_pb_copykun",
             "/app/bigpipe_log/dayu_newdsp_search_spam_pb_pipe",
             "/app/bigpipe_log/dayu_newdsp_spam_pb",
             "/app/bigpipe_log/dayu_newdsp_spam_pb_copykun",
             "/app/bigpipe_log/dayu_qiushi_search_normal_pb",
             "/app/bigpipe_log/dayu_qiushi_search_spam_pb",
             "/app/bigpipe_log/dayu_thirddsp_click_normal_txt",
             "/app/bigpipe_log/dayu_thirddsp_click_spam_txt",
             "/app/bigpipe_log/dayu_thirddsp_search_normal_pb",
             "/app/bigpipe_log/dayu_thirddsp_search_spam_pb",
             "/app/bigpipe_log/dmp_holmes",
             "/app/bigpipe_log/hdfs_read",
             "/app/bigpipe_log/nova-beidou-das",
             "/app/bigpipe_log/offline_beidou_join",
             "/app/bigpipe_log/offline_lu_join",
             "/app/bigpipe_log/offline_newdsp_join",
             "/app/bigpipe_log/offline_qiushi_join",
             "/app/bigpipe_log/offline_qiushi_join_wx",
             "/app/bigpipe_log/offline_thirddsp_join",
             "/app/bigpipe_log/orc",
             "/app/bigpipe_log/qiushi_winnotice_yxw",
             "/app/bigpipe_log/repair",
             "/app/bigpipe_log/test",
             "/app/bigpipe_log/tm_beidou_search_unjoin_pb",
             "/app/bigpipe_log/tm_holmes_join_pb",
             "/app/bigpipe_log/tm_lu_search_unjoin_pb",
             "/app/bigpipe_log/tm_newdsp_join_pb",
             "/app/bigpipe_log/tm_newdsp_search_unjoin_pb",
             "/app/bigpipe_log/tm_newdsp_search_unjoin_pb_copykun",
             "/app/bigpipe_log/tm_qiushi_search_unjoin_pb",
             "/app/bigpipe_log/tm_thirddsp_search_unjoin_pb"
          ],
          "timestamp":"1422336815247"
       },
       "result":"success",
       "message":""
    }
    ```


### mandone手动打done运行API

    http://hz01-cm-bsidx31.hz01.baidu.com:8482/mandone_run

* 参数： (除callback外，均为必填参数)

    callback: 伪json调用方式的js回调函数名，用于跨域访问

    bdir: 手动打done的目录名称（从mandone_list接口上获取）

    begin: 起始手动打done时间，格式如: "20150122 10:00"，必须为5分钟目录

    end  : 起始手动打done时间，格式如: "20150122 10:20"，必须为5分钟目录

    empty: 是否在hdfs目录放空标志的日志，如文本touchz空empty.log，二进制放大小为96的empty.seq，其值可以为empty、noempty、onlyempty、autoempty分别表示打done时放空seq、不放empty、只放empty不打done、自动判断是否需要放emtpy，建议放最后者，但速度比较慢

* 示例: 

    http://hz01-cm-bsidx31.hz01.baidu.com:8482/mandone_run?bdir=/app/bigpipe_log/offline_qiushi_join_wx&begin=20150124%2000:00&end=20150124%2002:00&myempty=empty

* 返回结果: 

    ```
    {  
       "data":{  
          "bdir":"/app/bigpipe_log/offline_qiushi_join_wx",
          "timestamp":"1422328010599"
       },
       "result":"success",
       "message":""
    }
    ```


### mandone手动打done查看运行进度API

    http://hz01-cm-bsidx31.hz01.baidu.com:8482/mandone_progress

* 参数:  (除callback外，均为必填参数)

    callback: 伪json调用方式的js回调函数名，用于跨域访问

    bdir: 手动打done的目录名称（从mandone_list接口上获取）

    begin: 起始手动打done时间，格式如: "20150122 10:00"，必须为5分钟目录

    end  : 起始手动打done时间，格式如: "20150122 10:20"，必须为5分钟目录

* 示例: 

    http://hz01-cm-bsidx31.hz01.baidu.com:8482/mandone_progress?bdir=/app/bigpipe_log/offline_qiushi_join_wx&begin=20150124%2000:00&end=20150124%2001:00

* 返回结果:

    ```
    {  
       "data":{  
          "bdir":"/app/bigpipe_log/offline_qiushi_join_wx",
          "timestamp":"1422337411983"
       },
       "result":"success",
       "rate":"0.33",
       "message":"0.33<br>"
    }
    ```


### mandone手动打done杀死任务API

    http://hz01-cm-bsidx31.hz01.baidu.com:8482/mandone_kill

* 参数: (除callback外，均为必填参数)

    callback: 伪json调用方式的js回调函数名，用于跨域访问

    bdir: 手动打done的目录名称（从mandone_list接口上获取）

* 示例: 

    http://hz01-cm-bsidx31.hz01.baidu.com:8482/mandone_kill?bdir=/app/bigpipe_log/offline_qiushi_join_wx

* 返回结果:

    ```
    {  
       "data":{  
          "bdir":"/app/bigpipe_log/offline_qiushi_join_wx",
          "timestamp":"1422328020354"
       },
       "result":"success",
       "message":""
    }
    ```
