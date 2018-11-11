#include<bits/stdc++.h>
using namespace std;
#define ll long long
#define rows 9
#define cols 6
#define user_ball_type 1
#define computer_ball_type 0
double ratio_x = 0.6;
struct node{
	ll x;
	ll y;
	ll ball_count;
	ll ball_type;
	ll threshold;
	vector<pair<ll,ll> > neighbours;
};
double maxi(double a,double b){
	if (a > b){
		return a;
	}
	return b;
}
ll random_value(ll min,ll max){
	static bool first = true;
	if (first){  
		srand( time(NULL) ); //seeding for the first time only!
		first = false;
	}
	return min + rand() % (( max + 1 ) - min);
}
node board[rows][cols];
bool iscorner(ll x,ll y){
	if (x%(rows-1) == 0 && y%(cols-1) == 0){
		return true;
	}
	return false;
}
bool isedge(ll x,ll y){
	if (x%(rows-1) == 0 || y%(cols-1) == 0 && !(x%(rows-1) == 0 && y%(cols-1) == 0)){
		return true;
	}
	return false;
}
bool iscenter(ll x,ll y){
	if (!iscorner(x,y) && !isedge(x,y)){
		return true;
	}
	return false;
}
bool exists(ll x,ll y){
	if (x >= 0 && x < rows && y >= 0 && y < cols){
		return true;
	}
	return false;
}
vector<pair<ll,ll> > getNeighbours(ll x,ll y){
	vector<pair<ll,ll> > neighbours;
	if (exists(x+1,y)){
		neighbours.push_back(make_pair(x+1,y));
	}
	if (exists(x,y+1)){
		neighbours.push_back(make_pair(x,y+1));
	}
	if (exists(x-1,y)){
		neighbours.push_back(make_pair(x-1,y));
	}
	if (exists(x,y-1)){
		neighbours.push_back(make_pair(x,y-1));
	}
	return neighbours;
}
void process(node board[][cols],ll x,ll y){
	queue<pair<ll,ll> >q;
	q.push(make_pair(x,y));
	while (q.size() > 0){
		pair<ll,ll> point = q.front();
		q.pop();
		if (board[point.first][point.second].ball_count >= board[point.first][point.second].threshold){
			vector<pair<ll,ll> >neighbours = board[point.first][point.second].neighbours;
			board[point.first][point.second].ball_count = 0;
			ll tmp_type = board[point.first][point.second].ball_type;	//change variable name
			board[point.first][point.second].ball_type = -1;
			for (ll k=0;k<(ll)neighbours.size();k++){
				board[neighbours[k].first][neighbours[k].second].ball_count += 1;
				board[neighbours[k].first][neighbours[k].second].ball_type = tmp_type;
				q.push(neighbours[k]);
			}
		}
	}
}
bool game_over(node board[][cols]){	//have to pass board as pointer
	set<ll>type;
	for (ll i=0;i<rows;i++){
		for (ll j=0;j<cols;j++){
			type.insert(board[i][j].ball_type);
		}
	}
	if ((ll)type.size() == 1){
		return true;
	}
	return false;
}
bool isemptycell(node board[][cols],ll x,ll y){
	if (board[x][y].ball_type == -1){
		return true;
	}
	return false;
}
void copy_board(node orig[][cols], node copy[][cols]){
	for (ll i=0;i<rows;i++){
		for (ll j=0;j<cols;j++){
			copy[i][j] = orig[i][j];
		}
	}
}
ll get_count_of_user_balls(node board[][cols]){
	ll count = 0;
	for (ll i=0;i<rows;i++){
		for (ll j=0;j<cols;j++){
			if (board[i][j].ball_type == user_ball_type){
				count += board[i][j].ball_count;
			}
		}
	}
	return count;
}
ll get_count_of_computer_balls(node board[][cols]){
	ll count = 0;
	for (ll i=0;i<rows;i++){
		for (ll j=0;j<cols;j++){
			if (board[i][j].ball_type == computer_ball_type){
				count += board[i][j].ball_count;
			}
		}
	}
	return count;
}
void init(){
	for (ll i=0;i<rows;i++){
		for (ll j=0;j<cols;j++){
			ll threshold = 0;
			board[i][j].ball_count = 0;
			board[i][j].x = i;
			board[i][j].y = j;
			board[i][j].ball_type = -1;
			if (iscorner(i,j)){
				threshold = 2;
			}
			else if (isedge(i,j)){
				threshold = 3;
			}
			else{
				threshold = 4;
			}
			board[i][j].threshold = threshold;
			board[i][j].neighbours = getNeighbours(i,j);
		}
	}
}
void print_board(node board[][cols]){
	for (ll i=0;i<rows;i++){
		for (ll j=0;j<cols;j++){
			if (board[i][j].ball_type == user_ball_type){
				cout << "\033[1;31m" << board[i][j].ball_count << "\033[0m" << " ";
			}
			else if (board[i][j].ball_type == computer_ball_type){
				cout << "\033[1;32m" << board[i][j].ball_count << "\033[0m" << " ";
			}
			else{
				cout << "\033[1;37m" << board[i][j].ball_count << "\033[0m" << " ";
			}
		}
		cout << endl;
	}
}
bool is_dominating(node board[][cols],ll x,ll y){
	vector<pair<ll,ll> >neighbours = board[x][y].neighbours;
	ll required = board[x][y].threshold - board[x][y].ball_count;
	for (ll k=0;k<(ll)neighbours.size();k++){
		if (board[x][y].ball_type != board[neighbours[k].first][neighbours[k].second].ball_type){
			if (board[neighbours[k].first][neighbours[k].second].threshold - board[neighbours[k].first][neighbours[k].second].ball_count < required){
				return false;
			}
		}
	}
	return true;
}
bool is_being_dominated(node board[][cols],ll x,ll y){
	return !is_dominating(board,x,y);
}
double get_future_gain(node board[][cols],ll x,ll y){
	vector<pair<ll,ll> > neighbours = board[x][y].neighbours;
	double future_gain = 0;
	for (ll k=0;k<(ll)neighbours.size();k++){
		if (board[neighbours[k].first][neighbours[k].second].ball_type != board[x][y].ball_type){
			future_gain += board[neighbours[k].first][neighbours[k].second].ball_count;
		}
	}
	return future_gain;
}
int main(){
	init();
	cout << "User plays first" << endl;
	bool user_turn = true;
	ll total_balls_count = 0;
	do{
		print_board(board);
		if (user_turn){
			ll x,y;
			do{
				cout << "Enter location of valid move" << endl;
				cin >> x >> y;
			}while(!(exists(x,y) && (board[x][y].ball_type == user_ball_type || isemptycell(board,x,y))));
			board[x][y].ball_count += 1;
			board[x][y].ball_type = user_ball_type;
			process(board,x,y);
		}
		else{
			double final_gain = -1;
			ll move_x,move_y;
			for (ll x=0;x<rows;x++){
				for (ll y=0;y<cols;y++){
					if (board[x][y].ball_type != user_ball_type){
						node tmpboard[rows][cols];
						copy_board(board,tmpboard);
						tmpboard[x][y].ball_count += 1;
						ll initial_user_ball_count = get_count_of_user_balls(tmpboard);
						process(tmpboard,x,y);
						ll final_user_ball_count = get_count_of_user_balls(tmpboard);
						double gain = initial_user_ball_count - final_user_ball_count;
						double gain_at_position = gain - (gain * (double)(final_user_ball_count)/(double)(total_balls_count + 1));   //in V2 have to change this to something better
						double future_gain = 0;
						if (gain_at_position == 0){
							if (is_dominating(board,x,y)){
								future_gain = get_future_gain(board,x,y);
							}
						}
						if (final_gain < gain_at_position + future_gain){
							final_gain = gain_at_position + future_gain;
							move_x = x;
							move_y = y;
						}
					}
				}
			}
			if (final_gain == 0){
				if (isemptycell(board,0,0)){
					move_x = 0;
					move_y = 0;
				}
				else if (isemptycell(board,0,cols-1)){
					move_x = 0;
					move_y = cols-1;
				}
				else if (isemptycell(board,rows-1,0)){
					move_x = rows-1;
					move_y = 0;
				}
				else if (isemptycell(board,rows-1,cols-1)){
					move_x = rows-1;
					move_y = cols-1;
				}
				else{
					vector<pair<ll,ll> >computer_moves;
					double total_computer_balls = 0;
					for (ll x=0;x<rows;x++){
						for (ll y=0;y<cols;y++){
							if (board[x][y].ball_type == computer_ball_type){
								total_computer_balls += board[x][y].ball_count;
								computer_moves.push_back(make_pair(x,y));
							}
						}
					}
					bool moved = false;
					if ((double)computer_moves.size()/total_computer_balls < ratio_x){
						cout << "ratio is " << (double)computer_moves.size()/total_computer_balls << endl;
						unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
						shuffle (computer_moves.begin(), computer_moves.end(), std::default_random_engine(seed));
						for (ll i=0;i<(ll)computer_moves.size();i++){
							if (is_being_dominated(board,computer_moves[i].first,computer_moves[i].second) || board[computer_moves[i].first][computer_moves[i].second].threshold - board[computer_moves[i].first][computer_moves[i].second].ball_count == 1){
								// do nothing
							}
							else{
								cout << "choosing shit" << endl;
								move_x = computer_moves[i].first;
								move_y = computer_moves[i].second;
								moved = true;
								break;
							}
						}
					}
					if (!moved){
						for (ll k=0;k<(ll)computer_moves.size();k++){
							vector<pair<ll,ll> >neighbours = board[computer_moves[k].first][computer_moves[k].second].neighbours;
							for (ll j=0;j<(ll)neighbours.size();j++){
								if (board[neighbours[j].first][neighbours[j].second].ball_type != user_ball_type){
									move_x = neighbours[j].first;
									move_y = neighbours[j].second;
									moved = true;
									break;
								}
							}
							if (moved){
								break;
							}
						}
					}
					if (!moved){
						do{
							move_x = random_value(0,rows);
							move_y = random_value(0,cols);
							cout << "choosing from random" << endl;
						}while(!(exists(move_x,move_y) && board[move_x][move_y].ball_type != user_ball_type));
					}
				}
			}
			board[move_x][move_y].ball_count += 1;
			board[move_x][move_y].ball_type = computer_ball_type;
			cout << "computer played at " << move_x << " " << move_y << endl;
			process(board,move_x,move_y);
		}
		user_turn = !user_turn;
		total_balls_count += 1;
	}while(!game_over(board));
	
	for (ll x=0;x<rows;x++){
		for (ll y=0;y<cols;y++){
			if (board[x][y].ball_type != -1){
				if (board[x][y].ball_type == user_ball_type){
					cout << "Congratulation's User for winning the game" << endl;
				}
				else{
					cout << "Computer Win's" << endl;
				}
				return 0;
			}
		}
	}
}
