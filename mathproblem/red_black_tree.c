#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Red-Black tree public method extern to use or private be invoke.
*/
#define PUBLIC
#define PRIVATE static

/**
* @brief Element compare EQ as equal, LT as little than, GT as greate than.
*/
#define EQ(a, b) (a == b)
#define LT(a, b) (a < b)
#define GT(a, b) (a > b)

/**
* @brief Element copy macro.
*
* @param des destination element.
* @param src source element.
*/
#define ELEMENT_COPY(des, src) { des = src; }

/**
* @brief Element type.
*/
typedef int ELEMENT;

typedef enum { BLACK = 0, RED = 1 } COLOR;

typedef struct _NODE {
	COLOR color;
	ELEMENT data;
	struct _NODE* lchild;
	struct _NODE* rchild;
} NODE, *TREE;

PUBLIC TREE	rbt_init(void);
PUBLIC void	rbt_destory(TREE T);
PUBLIC int	rbt_search(TREE T, ELEMENT d);
PUBLIC int	rbt_insert(TREE* pT, ELEMENT d, int* ptaller);
PUBLIC int	rbt_delete(TREE* pT, ELEMENT d, int* plower);
PUBLIC void	rbt_traveral(TREE T, void function(NODE*));

int main(int argc, const char *argv[])
{
	return 0;
}
