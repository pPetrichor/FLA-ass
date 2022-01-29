# include "turing.h"
using namespace std;
int main(int argc, char* argv[])
{
    char** command = argv;
    string file_name;
    string input;
    int r = command_read(argc,command,file_name,input);
    if(r == 3)
    {
        TM my_turing(file_name,false);
        int succ = my_turing.get_state();
        if(!succ) exit(1);
        my_turing.run(input,false);
        succ = my_turing.get_state();
        if(!succ) exit(1);
    }
    else if(r == 4)
    {
        TM my_turing(file_name,true);
        int succ = my_turing.get_state();
        if(!succ) exit(1);
        my_turing.run(input,true);
        succ = my_turing.get_state();
        if(!succ) exit(1);
    }
    else if(r == -1)
    {
        exit(1); //exit code is 1
    }
    
    return 0;
}