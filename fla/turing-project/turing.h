#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string.h>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;
int command_read(int c,char** v,string& file_name,string& input); //function for command process
struct id
{
    int step;
    int* index;//record the start of each tape
    string* tape;
    int* head;//record offset of tape start
    string state;
};
struct trans_way //the trans way for a certain delta
{
    string rewrite_symbols; //symbols that will take place of current_symbols
    string move; //tells how heads will move next
    string new_state;
};
struct delta
{
    map<pair<string,string>,trans_way> deltas;
    //pair<string,string> is <old_state,current_symbols(symbols processed by heads of types now)>,
    //the state before transforming with a certain delta
};

class TM
{
private:
    vector<string> Q; //states
    vector<char> S; //input symbols
    vector<char> G; //tape symbols
    string q_0; //start symbol
    char B; //blank symbol
    vector<string> F; //final states
    int n; //number of tapes
    vector<vector<string>> delta_read;
    delta trans; //trans functions
    bool normal; //tell if setup success
    id current_id; //record current id
    void get_states(string state);
    void process(string line,bool whether_v);
    void process_Q(string line,bool whether_v);
    void process_S(string line,bool whether_v);
    void process_G(string line,bool whether_v);
    void process_F(string line,bool whether_v);
    void process_delta(string line,bool whether_v);
    void get_input_symbols(char s);
    void get_tape_symbols(char s);
    void get_start_state(string state);
    void get_blank_symbol(char blank);
    void get_final_state(string state);
    void get_num(int x);
    void setup(string file_name,bool whether_v); //setup a turing machine
    void error_process(bool whether_v,string line,int kind);
    void check_grammar(bool whether_v);
    bool check_one_delta(vector<string> x,bool whether_v);
    void get_start_id(string input,bool whether_v);
    void show_id();
    int check_input(string input);
    bool search_trans(trans_way &x);
public:
    TM(string file_name,bool whether_v)
    {
        normal = true;
        setup(file_name,whether_v);
    }
    bool get_state();// tell if setup success
    void run(string input,bool whether_v);
    void print()
    {
        cout << "Q:"<< endl;
        for(int i=0;i<Q.size();++i) cout<<Q[i]<<" "<< endl;
        cout << "------------------------------"<< endl;
        cout << "S:"<< endl;
        for(int i=0;i<S.size();++i) cout<<S[i]<<" "<< endl;
        cout << "------------------------------"<< endl;
        cout << "G:"<< endl;
        for(int i=0;i<G.size();++i) cout<<G[i]<<" "<< endl;
        cout << "------------------------------"<< endl;
        cout << "q0:"<< endl;
        cout<<q_0<<" "<< endl;
        cout << "------------------------------"<< endl;
        cout << "B:"<< endl;
        cout<<B<<" "<< endl;
        cout << "------------------------------"<< endl;
        cout << "n:"<< endl;
        cout<<n<<" "<< endl;
        cout << "------------------------------"<< endl;
        cout << "normal:"<< endl;
        cout<<normal<<" "<< endl;
        cout << "------------------------------"<< endl;
        cout << "F:"<< endl;
        for(int i=0;i<F.size();++i) cout<<F[i]<<" "<< endl;
        cout << "------------------------------"<< endl;
        cout << "F:"<< endl;
        
        map<pair<string,string>,trans_way> ::iterator it;
        for(it = trans.deltas.begin();it!=trans.deltas.end();++it)
            cout << it->first.first << " " << it->first.second << " " << it->second.rewrite_symbols << " " << it->second.move << " " <<it->second.new_state << endl;

    }
};