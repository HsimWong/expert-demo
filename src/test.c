#include <stdio.h>
#include <string.h>
// #include "node.h"
// #include "conf.h"
#include "utils.h"
// #include <stdlib.h>
// #define MAX_JOBS 0xff
// #define MAX_NODES 0xf


// 
int main(int argc, char const *argv[]) {
    // Job ** jobs = (Job **)malloc(MAX_JOBS * sizeof(Job *));
    // Node * node_master = (Node *)malloc(sizeof(Node *));
    // Node * node_backup = (Node *)malloc(sizeof(Node *));

    // InitNode(node_master, jobs,
    //          conf.main_node_addr, conf.display);
    // InitNode(node_backup, jobs,
    //          conf.back_node_addr, conf.display);
    printf("FUCK");
    if (check_connectivity("baidu.com")) {
        printf("BAIDU OK\n");
    }


    
    if (!check_connectivity("google.com")) {
        printf("GOOGLE not ok\n");
    }

    printf("%d", exec_local("firefox"));
    return 0;
}
