
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
using namespace std;

int MIN_BITS = 4;		//minimum bits to print
vector<unsigned> input_values;	
bool show_mid = false;		//show middle process

struct B_number{
	unsigned number;
	unsigned dashes;
	bool used;
};

vector<vector<B_number> > table;
vector<vector<B_number> > p_group;
vector<vector<B_number> > final_group;
vector<B_number> printed_numbers;

//----------------------------------------------------------
unsigned count_1s(unsigned number);
void print_binary(unsigned number);
void create_table();
void print_table();
B_number init_B_number(unsigned n,int d, bool u);
void create_p_group();
void print_p_group();
void print_p_binary(unsigned n, unsigned d);
void create_final_group();
void print_final_group();
bool is_printed(B_number n);
void init();
void getinput();
unsigned count_bits(unsigned n);
//----------------------------------------------------------

int main(int argc, char *argv[]) {

	/* allow command line calling with arguments -m -b X
	   where X is a number. order or -m and -b X does not
	   matter*/
	cout<<"\nQMCS - Quine McCluskey Simplifier\n";
	if(argc >= 2)
	{
		string arg = argv[1];
		if(arg.find("-m") != -1) {
			show_mid = true;
			if(argc>=3) {
				arg = argv[2];
				if(arg.find("-b") != -1)
					MIN_BITS = atoi(argv[3]);
			}
		}
		else if(arg.find("-h") != -1) {
			cout<<"-b X\tminimum bits should be X.\n"
			    <<"-m  \tshow mid process computation.\n"
			    <<"-h  \tshow this.\n";
			return 0;
		
		}
		else {
			if(arg.find("-b") != -1  && argc >=3)
				MIN_BITS = atoi(argv[2]);

			if(argc >=4) {
			       	arg = argv[3];
				if(arg.find("-m") != -1) 
					show_mid = true;
			}
			else
			{
				cout<<"Invalid argument\n"
				    <<"-b X\tminimum bits should be X.\n"
				    <<"-m  \tshow mid process computation.\n"
				    <<"-h  \tshow this.\n";
				return 0;
			}
		}
	}

	getinput();
	init();
	
	return 0;
	
}


unsigned count_1s(unsigned number) {
	short bit =0;
	int count = 0;
	while(number>0)	{
		bit = number%2;
		number>>=1;
		if(bit) {
			count++;
		}
	}
	return count;
}

void print_binary(unsigned number) {
	unsigned bits[MIN_BITS];
	int count = 0;
	
	while(number>0||count<MIN_BITS)	{
		bits[count] = number%2;
		number>>= 1;
		count++;
	}
	for(int i=count-1;i>=0;i--)
		cout<<bits[i];
}

void create_table() {
	short tmp;
	B_number temp_num;
	for(int i=0;i<input_values.size();i++) {
		tmp = count_1s(input_values[i]);
		if(tmp+1>table.size())
			table.resize(tmp+1);
		
		temp_num = init_B_number(input_values[i],0,false);
		table[tmp].push_back(temp_num);
	}
}

void print_table() {
	
	cout<<endl<<"COMPUTING:"<<endl;
	for(int i=0;i<table.size();i++)	{
		cout<<i;
		for(int j=0;j<table[i].size();j++) {
			cout<<"\tm"<<table[i][j].number<<"\t";
			print_binary(table[i][j].number);
			cout<<endl;
		}
		cout<<"\n-------------------------------------"<<endl;
	}
}
B_number init_B_number(unsigned	n,int d, bool u) {
	B_number num;
	num.number = n;
	num.dashes = d;
	num.used = u;
	return num;
}

void create_p_group() {
	short tmp;
	B_number temp_num;
	unsigned temp_number, temp_dashes;
	for(int i=0;i<table.size()-1;i++) {
		for(int j=0;j<table[i].size();j++) {
			for(int k=0;k<table[i+1].size();k++) {
				temp_number = table[i][j].number & table[i+1][k].number;
				temp_dashes = table[i][j].number ^ table[i+1][k].number;
				
				if(count_1s(temp_dashes)==1) {
					table[i][j].used = true;
					table[i+1][k].used = true;
					
					
					tmp = count_1s(temp_number);
					
					if(tmp+1>p_group.size())
						p_group.resize(tmp+1);
					
					temp_num = init_B_number(temp_number, temp_dashes, false);
					p_group[tmp].push_back(temp_num);
				}
			}
		}
	}
}

void print_p_group() {
	cout<<endl<<"MID PROCESS COMPUTATION:"<<endl;
	
	for(int i=0;i<p_group.size();i++) {
		cout<<i;
		for(int j=0;j<p_group[i].size();j++) {
			cout<<"\t\t";
			print_p_binary(p_group[i][j].number,p_group[i][j].dashes);
			cout<<endl;
		}
		cout<<"\n-------------------------------------"<<endl;
	}
	
}

void print_p_binary(unsigned n, unsigned d) {
	unsigned bits[MIN_BITS];
	int count = 0;
	
	while(n>0||count<MIN_BITS) {
		if(!(d%2))
			bits[count] = n%2;
		else
			bits[count] = 2;
		
		n >>= 1;
		d >>= 1;
		count++;
	}
	for(int i=count-1;i>=0;i--) {
		if(bits[i]!=2)
			cout<<bits[i];
		else
			cout<<"-";
	}
}

void create_final_group() {
	short tmp;
	B_number temp_num;
	unsigned temp_number, temp_dashes;
	for(int i=0;i<p_group.size()-1;i++) {
		for(int j=0;j<p_group[i].size();j++) {
			for(int k=0;k<p_group[i+1].size();k++) {
				if(p_group[i][j].dashes == p_group[i+1][k].dashes) {
					temp_number = p_group[i][j].number & p_group[i+1][k].number;
					temp_dashes = p_group[i][j].number ^ p_group[i+1][k].number;
					if(count_1s(temp_dashes)==1) {
						temp_dashes^= p_group[i][j].dashes;
						
						p_group[i][j].used = true;
						p_group[i+1][k].used = true;
						
						tmp = count_1s(temp_number);
						
						if(tmp+1>final_group.size())
							final_group.resize(tmp+1);
						
						temp_num = init_B_number(temp_number, temp_dashes, true);
						final_group[tmp].push_back(temp_num);
					}
				}
			}
		}
	}
}

void print_final_group() {
	cout<<endl<<"FINAL:\n-------------------------------------"<<endl;
	int i,j;
	for(i=0;i<final_group.size();i++) {
		for(j=0;j<final_group[i].size();j++) {
			if(!is_printed(final_group[i][j])) {
				print_p_binary(final_group[i][j].number,final_group[i][j].dashes);
				cout<<endl;
				printed_numbers.push_back(final_group[i][j]);
			}
		}
	}
	for(i=0;i<p_group.size();i++) {
		for(j=0;j<p_group[i].size();j++) {
			if(!p_group[i][j].used) {
				print_p_binary(p_group[i][j].number,p_group[i][j].dashes);
				cout<<endl;
			}
		}
	}
	for(i=0;i<table.size();i++) {
		for(j=0;j<table[i].size();j++) {
			if(!table[i][j].used) {
				print_p_binary(table[i][j].number,table[i][j].dashes);
				cout<<endl;
			}
		}
	}
	cout<<"-------------------------------------"<<endl;
}
bool is_printed(B_number n) {
	for(int i=0;i<printed_numbers.size();i++)
		if(n.number==printed_numbers[i].number && n.dashes == printed_numbers[i].dashes)
			return true;
	
	return false;
}
void init() {
	table.resize(1);
	p_group.resize(1);
	final_group.resize(1);
	create_table();
	print_table();
	create_p_group();
	if(show_mid)
		print_p_group();
	create_final_group();
	print_final_group();
}

void getinput() {
	unsigned in;
	int num_bits=0;
	cout<<"\nInput value followed by ENTER[^D ends input]\n> ";
	while(cin>>in) {
		input_values.push_back(in);
		num_bits = count_bits(in);	
		if(num_bits>MIN_BITS)
			MIN_BITS = num_bits;
		cout<<"> ";
	}
}

unsigned count_bits(unsigned n) {
	short bit =0;
	int count = 0;
	while(n>0) {
		bit = n%2;
		n>>=1;
		count++;
	}
	return count;
}
