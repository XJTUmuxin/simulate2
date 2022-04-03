#include<bits/stdc++.h>
using namespace std;
string init_seq_path = "./../init_seq/";
string long_sub_seq_path = "./../long_sub_seq/";
string short_sub_seq_path = "./../short_sub_seq/";
int expect_len = 100000;   
int long_sub_len = 10000;
int short_sub_len = 100;
int long_sub_num = 101;
int short_sub_num = 1000;
int min_mic_len = 100;
int max_mic_len = 1000;
int mic_num = 10;
int error_rate = 15;
char bases[5] = {'A','C','G','T','N'};
vector<string> motifs = {"AC","TCAT","AGAG","ATCATC","TATATA","AGT","TTG","CGC","TG","AT"};

void create_init_seq(string& init_seq){
    vector<int> m_indexs(mic_num);
    for(int i=0;i<mic_num;++i){
        int temp = rand()%expect_len;
        m_indexs[i] = temp;
    }
    sort(m_indexs.begin(),m_indexs.end());
    m_indexs.push_back(INT_MAX);
    int m_num = 0;
    int m_index = m_indexs[m_num];
    ofstream ofs(init_seq_path+"expect_mic.out");
    while(init_seq.size()<expect_len || mic_num>0){
        if(init_seq.size()>m_index){
            string motif = motifs[m_num];
            int repeat_num = (min_mic_len+rand()%(max_mic_len-min_mic_len))/motif.size();
            ofs<<"index: "<<init_seq.size()<<endl<<"motif: "<<motif<<endl<<"repeat_num: "<<repeat_num<<endl;
            for(int i=0;i<repeat_num;++i){
                init_seq += motif;
            }
            mic_num--;
            m_index = m_indexs[++m_num];
        }
        int temp = rand()%4;
        init_seq += bases[temp];
    }
    ofs<<"seq_size: "<<init_seq.size()<<endl;
    ofs.close();
    ofs.clear();
} 



void get_short_substrings(string& init_seq,vector<string>& substrings){
    for(int i=0;i<short_sub_num;++i){
        int start_index;
        while(true){
            start_index = rand()%init_seq.size();
            if(start_index+short_sub_len<=init_seq.size())break;
        }
        substrings.emplace_back(init_seq,start_index,short_sub_len);
    }
}

void get_long_substrings(string& init_seq,vector<string>& substrings){
    for(int i=0;i<long_sub_num;++i){
        int start_index;
        while(true){
            start_index = rand()%init_seq.size();
            if(start_index+long_sub_len<=init_seq.size())break;
        }
        string str = init_seq.substr(start_index,long_sub_len);
        for(auto& ch:str){
            int temp = rand()%100;
            if(temp<error_rate){
                char base;
                while(true){
                    base = bases[rand()%5];
                    if(base!=ch)break;
                }
                ch = base;
            }
        }
        substrings.push_back(str);
    }
}

int main(){
    
    srand(static_cast<unsigned>(time(0)));
    string init_seq = "";
    vector<string> long_substrings;
    vector<string> short_substrings;
    create_init_seq(init_seq);
    get_long_substrings(init_seq,long_substrings);
    get_short_substrings(init_seq,short_substrings);
    ofstream ofs(init_seq_path+"init_seq.fasta");
    ofs<<">init_seq"<<endl;
    ofs<<init_seq;
    ofs.close();
    ofs.clear();
    ofs.open(short_sub_seq_path+"short_sub_seq.fasta");
    int number = 1;
    for(auto &str:short_substrings){
        ofs<<(">Sread"+to_string(number))<<endl;
        ofs<<str<<endl;
        number++;
    }
    ofs.close();
    ofs.clear();
    number = 1;
    ofs.open(long_sub_seq_path+"long_sub_seq.fasta");
    for(auto &str:long_substrings){
        ofs<<(">Lread"+to_string(number))<<endl;
        ofs<<str<<endl;
        number++;
    }
    ofs.close();
    ofs.clear();
}
