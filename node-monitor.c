#include "utils.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

DBUpdate *dbhandler;

int main(int argc, char const *argv[]) {
    DBUpdate *dbhandler = InitDBUpdate();
    char *nodes[] = {"127.0.0.1", "192.168.122.82", "baidu.com"};
    dbhandler = InitDBUpdate();
    while (1) {
        for (int i = 0; i < (sizeof(nodes)/sizeof(char *)); i++) {
            char *status = check_connectivity(nodes[i]) ? "OK" : "FAILED";
            dbhandler->update_status(dbhandler, nodes[i], status);
        }
        sleep(5);
    }
    return 0;
}
