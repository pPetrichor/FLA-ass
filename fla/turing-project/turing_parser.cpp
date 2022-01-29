#include "turing.h"
void TM::error_process(bool whether_v,string line,int kind)
{
    if(!whether_v)
        cerr << "syntax error" << endl;
    else
    {
        switch (kind)
        {
        case 1:
            cerr << "syntax error :" << line << " doesn't match \'#q0 = q\'" << endl;
            break;
        case 2:
            cerr << "syntax error :" << line << "; as a delta the num of parameters is wrong" << endl;
            break;
        case 3:
            cerr << "syntax error :no such grammar: " << line << endl;
            break;
        case 4:
            cerr << "syntax error :" << line << " doesn't match \'#B = b\'" << endl;
            break;
        case 5:
            cerr << "syntax error :" << line << " doesn't match \'#N = n\'" << endl;
            break;
        case 6:
            cerr << "syntax error :" << line << " doesn't match \'#N = n\', n has nonnumeric character" << endl;
            break;
        case 7:
            cerr << "syntax error :no such grammar: " << line << endl;
            break;
        case 8:
            cerr << "syntax error :" << line << " doesn't match \'#Q = {q1,q2,...,qi}\'" << endl;
            break;
        case 9:
            cerr << "syntax error :" << "nothing exits before \'}\';the last char in line:" << line << endl;
            break;
        case 10:
            cerr << "syntax error :" << line << " doesn't match \'#S = {s1,s2,...,sj}\'" << endl;
            break;
        case 11:
            cerr << "syntax error :" << "not char but string appears in S: " << line << endl;
            break;
        case 12:
            cerr << "syntax error :" << line << " doesn't match \'#G = {s1,s2,...,sk}\'" << endl;
            break;
        case 13:
            cerr << "syntax error :" << "not string but char appears in G: " << line << endl;
            break;
        case 14:
            cerr << "syntax error :" << line << " doesn't match \'#F = {f1,f2,...,fn}\'" << endl;
            break;
        case 15:
            cerr << "syntax error :\'" << line << "\' length < 6, please check your content." << endl;
            break;
        case 16:
            cerr << "syntax error :\'" << line << "\' length < 7, match fault." << endl;
            break;
        case 17:
            cerr << "syntax error :\'" << line << "\' length != 7, match fault." << endl;
        }
    }
    normal = false;
}
void TM::check_grammar(bool whether_v)
{
    //check if S in G
    int i = 0,j = 0;
    for(i=0;i<S.size();++i)
    {
        for(j=0;j<G.size();++j)
        {
            if(S[i] == G[j]) break;
        }
        if(j == G.size())
        {
            normal = false;
            if(!whether_v)
                {cerr << "syntax error" << endl;return;}
            else
                cerr << "syntax error :" << S[i] << " appears in S but not in G" << endl;
        }
    }
    //check if q0 in Q
    bool if_exit = false;
    for(i=0; i<Q.size();++i)
    {
        if(Q[i] == q_0)
        {
            if_exit = true;
            break;
        }
    }
    if(!if_exit) //q0 is not in Q
    {
        normal = false;
        if(!whether_v)
            {cerr << "syntax error" << endl;return;}
        else
            cerr << "syntax error :" << q_0 << " appears in q0 but not in Q" << endl;
    }
    //check if B in G
    if_exit = false;
    for(i=0; i<G.size();++i)
    {
        if(G[i] == B)
        {
            if_exit = true;break;
        }
    }
    if(!if_exit) //B is not in G
    {
        normal = false;
        if(!whether_v)
            {cerr << "syntax error" << endl;return;}
        else
            cerr << "syntax error :" << B << " is blank symbol but not in G" << endl;
    }
    //check if F in Q
    for(i=0; i<F.size();++i)
    {
        for(j=0;j<Q.size();++j)
        {
            if(F[i] == Q[j]) break;
        }
        if(j == Q.size())
        {
            normal = false;
            if(!whether_v)
                {cerr << "syntax error" << endl;return;}
            else
                cerr << "syntax error :" << F[i] << " appears in F but not in Q(maybe something wrong in \'#Q = {...}\' leads to this error)" << endl;
        }
    }
    //check delta
    for(int i = 0;i < delta_read.size();++i)
    {
        bool res = check_one_delta(delta_read[i],whether_v);
        if(res) //delta is legal
        {
            trans_way t;
            t.rewrite_symbols = delta_read[i][2];
            t.move = delta_read[i][3];
            t.new_state = delta_read[i][4];
            pair<string,string> t1 = make_pair(delta_read[i][0],delta_read[i][1]);
            trans.deltas[t1] = t;//add a delta
        }
        else if(!whether_v)
            break;
    }
}
int command_read(int c,char** v,string& file_name,string& input)
{
    int res = 0;
    if(c > 1 && (strcmp(v[1],"-h") == 0 || strcmp(v[1],"--help") == 0))
    {
        cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
        res = 2;
    }
    else if(c > 2 && (strcmp(v[2],"-h") == 0 || strcmp(v[2],"--help") == 0))
    {
        cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
        res = 2;
    }
    else
    {
        switch(c)
        {
            case 2: //should be turing [-h|--help]
                if(strcmp(v[1],"-h") == 0 || strcmp(v[1],"--help") == 0)
                {
                    cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
                    res = 2;
                }
                else if(strcmp(v[1],"-v") == 0 || strcmp(v[1],"--verbose") == 0)
                {
                    cerr << "lack <tm> <input>; try \"turing -h|--help\"" << endl;
                    res = -1;
                }
                else
                {
                    cerr << "no such command; try \"turing -h|--help\"" << endl;
                    res = -1;
                }
                break;
            case 3: //should be turing <tm> <input>
                file_name = v[1];
                input = v[2];
                res = 3;
                break;
            case 4: //should be turing [-v|--verbose] <tm> <input>
                if(strcmp(v[1],"-v") == 0 || strcmp(v[1],"--verbose") == 0)
                {
                    file_name = v[2];
                    input = v[3];
                    res = 4;
                }
                else
                {
                    cerr << "no such command; try \"turing -h|--help\"" << endl;
                    res = -1;
                }
                break;
            default:
                cerr << "no such command; try \"turing -h|--help\"" << endl;
                res = -1;
        }
    }
    
    return res;
}
void TM::setup(string file_name,bool whether_v)
{
    fstream fin(file_name);
    if(!fin)
    {
        cerr << "open file failed! Please use \'turing -h|--help\' to check your command or check whether filename is right.\n";
        normal = false;
        return;
    }
    else
    {
        while(!fin.eof())
        {
            string current_line = "";
            string line;
            getline(fin,line); //read one line
            int start = 0;//remove blanks ahead
            for(;start<line.length();++start)
            {
                if(line[start] != ' ') break;
            }
            for(;start<line.length();++start) current_line += line[start];
            if(current_line[0] != ';' && current_line != "")
            {
                int end = current_line.find(';');
                if(end != -1) //note exits
                    current_line = current_line.assign(current_line.c_str(),end); //ignore notes
                int length = current_line.length();
                for(int i = current_line.length() - 1; i >= 0; --i)
                {
                    if(current_line[i] == ' ')
                        length--;
                    else
                        break;
                }
                //check whether current_line is empty and remove blanks in the end of the sting
                if(length > 0)
                {
                    current_line = current_line.assign(current_line.c_str(),length);
                    process(current_line,whether_v);
                    
                }
            }
            if(!whether_v && !normal) return;
        }
        check_grammar(whether_v);
    }
}
void TM::process(string line,bool whether_v)
{
    if(line[0] == '#')
    {
        if(line.length() < 6)
        {
            error_process(whether_v,line,15);
            return;
        }
        switch(line[1])
        {
            case 'Q':
                if(line.length() < 7)
                {
                    error_process(whether_v,line,16);
                    return;
                }
                process_Q(line,whether_v);
                break;
            case 'S':
                if(line.length() < 7)
                {
                    error_process(whether_v,line,16);
                    return;
                }
                process_S(line,whether_v);
                break;
            case 'G':
                if(line.length() < 7)
                {
                    error_process(whether_v,line,16);
                    return;
                }
                process_G(line,whether_v);
                break;
            case 'q':
                if(line.length() < 7)
                {
                    error_process(whether_v,line,16);
                    return;
                }
                if(line[2] == '0')
                {
                    if(line[3] != ' ' || line[4] != '=' || line[5] != ' ')
                        error_process(whether_v,line,1);
                    else
                    {
                        string s = "";
                        for(int i = 6; i < line.length(); ++i)
                            s += line[i];
                        get_start_state(s);
                    }
                }
                else
                    error_process(whether_v,line,3);
                break;
            case 'B':
                if(line.length() != 6)
                {
                    error_process(whether_v,line,17);
                    return;
                }
                if(line[2] != ' ' || line[3] != '=' || line[4] != ' ')
                    error_process(whether_v,line,4);
                else
                    get_blank_symbol(line[5]);
                break;
            case 'F':
                if(line.length() < 7)
                {
                    error_process(whether_v,line,16);
                    return;
                }
                process_F(line,whether_v);
                break;
            case 'N':
                if(line[2] != ' ' || line[3] != '=' || line[4] != ' ')
                    error_process(whether_v,line,5);
                else
                {
                    int num = 0;bool f = true;
                    for(int i=5; i<line.length();++i)
                    {
                        if(line[i] >= '0' && line[i] <= '9')
                        {
                            num *= 10;
                            num += line[i] - '0';
                        }
                        else
                        {
                            f = false;
                            error_process(whether_v,line,6);
                            break;
                        }
                    }     
                    if(f) get_num(num);
                }
                break;
            default:
                error_process(whether_v,line,7);
        }
    }
    else
    {
        //process delta
        process_delta(line,whether_v);
    }
    
}
void TM::process_Q(string line,bool whether_v)
{
    if(line[2] != ' ' || line[3] != '=' || line[4] != ' ' || line[5] != '{' || line[line.length()-1] != '}')
        error_process(whether_v,line,8);
    else
    {
        string s = "";
        for(int i = 6; i < line.length(); ++i)
        {
            if(i == line.length()-1)
            {
                if(s.length() == 0) //nothing exits before '}'
                    {
                        error_process(whether_v,line,9);
                        return;
                    }
                else
                {
                    get_states(s);
                    s = "";
                }
            }
            else if(line[i] == ',')
            {
                if(s.length() == 0) //nothing exits before current ','
                {
                    if(!whether_v)
                        cerr << "syntax error" << endl;
                    else
                        cerr << "syntax error :" << "nothing exits before \',\': the " << i << "th char in line:" << line << endl;
                    normal = false;
                    return;
                }
                else
                {
                    get_states(s);
                    s = "";
                }
            }
            else if((line[i] >= 'a' && line[i] <= 'z')||(line[i] >= 'A' && line[i] <= 'Z')||(line[i] >= '0' && line[i] <= '9')||line[i] == '_')
                s += line[i];
            else
            {
                if(!whether_v)
                    cerr << "syntax error" << endl;
                else
                    cerr << "syntax error :" << "wrong char \'" << line[i] << "\' appears in line:" << line << endl;
                normal = false;
                return;
            }
        }
    }
}
void TM::process_S(string line,bool whether_v)
{
    if(line[2] != ' ' || line[3] != '=' || line[4] != ' ' || line[5] != '{' || line[line.length()-1] != '}')
        error_process(whether_v,line,10);
    else
    {
        char s = ' ';
        for(int i = 6; i < line.length(); ++i)
        {
            if(i == line.length()-1)
            {
                if(s == ' ') //nothing exits before '}'
                    {
                        error_process(whether_v,line,9);
                        return;
                    }
                else
                {
                    get_input_symbols(s);
                    s = ' ';
                } 
            }
            else if(line[i] == ',')
            {
                if(s == ' ') //nothing exits before current ','
                {
                    if(!whether_v)
                        cerr << "syntax error" << endl;
                    else
                        cerr << "syntax error :" << "nothing exits before \',\';the " << i << "th char in line:" << line << endl;
                    normal = false;return;
                }
                else
                {
                    get_input_symbols(s);
                    s = ' ';
                }  
            }
            else if(line[i] >= 32 && line[i] <= 126 && line[i] != ' ' && line[i] != ';' && line[i] != '*' && line[i] != '_')
            {
                if(s != ' ') //string but char appears in S
                    {
                        error_process(whether_v,line,11);
                        return;
                    }
                else s = line[i];
            }
            else
            {
                if(!whether_v)
                    cerr << "syntax error" << endl;
                else
                    cerr << "syntax error :" << "wrong char \'" << line[i] << "\' appears in line:" << line << endl;
                normal = false;
                return;
            }
        }
    }
}
void TM::process_G(string line,bool whether_v)
{
    if(line[2] != ' ' || line[3] != '=' || line[4] != ' ' || line[5] != '{' || line[line.length()-1] != '}')
        error_process(whether_v,line,12);
    else
    {
        char s = ' ';
        for(int i = 6; i < line.length(); ++i)
        {
            if(i == line.length()-1)
            {
                if(s == ' ') //nothing exits before '}'
                    {
                        error_process(whether_v,line,9);
                        return;
                    }
                else
                {
                    get_tape_symbols(s);
                    s = ' ';
                } 
            }
            else if(line[i] == ',')
            {
                if(s == ' ') //nothing exits before current ','
                {
                    if(!whether_v)
                        cerr << "syntax error" << endl;
                    else
                        cerr << "syntax error :" << "nothing exits before \',\';the " << i << "th char in line:" << line << endl;
                    normal = false;return;
                }
                else
                {
                    get_tape_symbols(s);
                    s = ' ';
                }
            }
            else if(line[i] >= 32 && line[i] <= 126 && line[i] != ' ' && line[i] != ';' && line[i] != '*')
            {
                if(s != ' ') //string but char appears in G
                    {
                        error_process(whether_v,line,13);
                        return;
                    }
                else s = line[i];
            }
            else
            {
                if(!whether_v)
                    cerr << "syntax error" << endl;
                else
                    cerr << "syntax error :" << "wrong char \'" << line[i] << "\' appears in line:" << line << endl;
                normal = false;return;
            }
        }
    }
}
void TM::process_F(string line,bool whether_v)
{
    if(line[2] != ' ' || line[3] != '=' || line[4] != ' ' || line[5] != '{' || line[line.length()-1] != '}')
    {
        error_process(whether_v,line,14);
    }
    else
    {
        string s = "";
        for(int i = 6; i < line.length(); ++i)
        {
            if(i == line.length()-1)
            {
                if(s.length() == 0) //nothing exits before '}'
                {
                    error_process(whether_v,line,9);
                    return;
                }
                else
                {
                    get_final_state(s);
                    s = "";
                }
                
            }
            else if(line[i] == ',')
            {
                if(s.length() == 0) //nothing exits before current ','
                {
                    if(!whether_v)
                        cerr << "syntax error" << endl;
                    else
                        cerr << "syntax error :" << "nothing exits before \',\';the " << i << "th char in line:" << line << endl;
                    normal = false;return;
                }
                else
                {
                    get_final_state(s);
                    s = "";
                }
                
            }
            else if((line[i] >= 'a' && line[i] <= 'z')||(line[i] >= 'A' && line[i] <= 'Z')||(line[i] >= '0' && line[i] <= '9')||line[i] == '_')
                s += line[i];
            else
            {
                if(!whether_v)
                    cerr << "syntax error" << endl;
                else
                    cerr << "syntax error :" << "wrong char \'" << line[i] << "\' appears in line:" << line << endl;
                normal = false;
                return;
            }
        }
    }
}
void TM::get_states(string state)
{
    Q.push_back(state);
}
bool TM::get_state()
{
    return normal;
}
void TM::get_input_symbols(char s)
{
    S.push_back(s);
}
void TM::get_tape_symbols(char s)
{
    G.push_back(s);
}
void TM::get_start_state(string state)
{
    q_0 = state;
}
void TM::get_blank_symbol(char blank)
{
    B = blank;
}
void TM::get_final_state(string state)
{
    F.push_back(state);
}
void TM::get_num(int x)
{
    n = x;
}
void TM::process_delta(string line,bool whether_v)
{
    vector<string> d;
    stringstream stin(line);
    string temp;
    int i = 0;
    while(stin >> temp)
    {
        d.push_back(temp);
    }
    if(d.size() != 5)
    {
        error_process(whether_v,line,2);
        return;
    }
    delta_read.push_back(d);
}
bool TM::check_one_delta(vector<string> x,bool whether_v)
{
    //check current state
    string s = x[0];
    bool if_exit = false;
    bool res = true;
    for(int i=0; i<Q.size();++i)
    {
        if(Q[i] == s)
        {
            if_exit = true;
            break;
        }
    }
    if(!if_exit) //s is not in Q
    {
        normal = false;
        if(!whether_v)
           {cerr << "syntax error" << endl;return false;}
        else
        {
            cerr << "syntax error :state " << s << " appears in delta:";
            for(int i=0;i<x.size();++i) cout << x[i] << ' ';
            cerr << "but not in Q" << endl;
        }
        res = false;
    }
    //check new state
    s = x[4];
    if_exit = false;
    for(int i=0; i<Q.size();++i)
    {
        if(Q[i] == s)
        {
            if_exit = true;
            break;
        }
    }
    if(!if_exit) //s is not in Q
    {
        normal = false;
        if(!whether_v)
           {cerr << "syntax error" << endl;return false;}
        else
        {
            cerr << "syntax error :state " << s << " appears in delta:";
            for(int i=0;i<x.size();++i) cout << x[i] << ' ';
            cerr << "but not in Q" << endl;
        }
        res = false;
    }
    //check current symbol and rewrite symbols
    int times = 0;
    while(times < 2)
    {
        times++;
        s = x[times];
        if(s.length() != n)
        {
            normal = false;
            if(!whether_v)
            {cerr << "syntax error" << endl;return false;}
            else
            {
                cerr << "syntax error : symbols " << s << " don't match tape number in delta:";
                for(int i=0;i<x.size();++i) cout << x[i] << ' ';
                cerr << endl;
            }
            res = false;
        }
        for(int i=0;i<s.length();++i)
        {
            char c = s[i];
            int j = 0;
            for(; j<G.size();++j)
            {
                if(G[j] == c)
                    break;
            }
            if(j == G.size())
            {
                normal = false;
                if(!whether_v)
                    {cerr << "syntax error" << endl;return false;}
                else
                {
                    cerr << "syntax error :" << c << " is current/rewrite symbol in delta: ";
                    for(int i=0;i<x.size();++i) cout << x[i] << ' ';
                    cerr << ", but not in G" << endl;
                } 
            }
        }
    }
    //check move
    s = x[3];
    if(s.length() != n)
    {
        normal = false;
        if(!whether_v)
        {cerr << "syntax error" << endl;return false;}
        else
        {
            cerr << "syntax error :move symbols " << s << " don't match tape number in delta:";
            for(int i=0;i<x.size();++i) cout << x[i] << ' ';
            cerr << endl;
        }
        res = false;
    }
    for(int i=0;i<s.length();++i)
    {
        char c = s[i];
        if(c != 'l' && c != 'r' && c != '*')
        {
            normal = false;
            if(!whether_v)
                {cerr << "syntax error" << endl;return false;}
            else
            {
                cerr << "syntax error :" << c << " is move symbol in delta: ";
                for(int i=0;i<x.size();++i) cout << x[i] << ' ';
                cerr << ", but not l or r or *" << endl;
            } 
        }
    }
    return res;
}