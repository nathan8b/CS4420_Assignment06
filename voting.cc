#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>   
using namespace std;

int main() {
    // get number of voters and rounds from the user
    int n;
    int rounds;
    cout << "Enter the number of voters: ";
    cin >> n;
    cout << "Enter the number of rounds: ";
    cin >> rounds;

    // judge create n pipes
    int fd[n][2];
    for(int i = 0; i < n; i++){
        pipe(fd[i]);
    }

    // judge forks n voters
    int id = -1;
    for(int i = 0; i < n; i++){
        if(fork() == 0){
            id = i;
            break;
        }
    }

    // give each voter its own random seed
    if(id != -1){
        srand(getpid());
    }

    // loop through each round, getting the vote from each pipe
    for(int r = 1; r <= rounds; r++){
        // judge
        if(id == -1){
            int yes = 0;
            int no = 0;
            // read a vote from each voter's pipe and count it
            for(int i = 0; i < n; i++){
                int vote;
                read(fd[i][0], &vote, sizeof(vote));
                if(vote == 1)
                    yes++;
                else
                    no++;
            }
            // print the majority result
            if(yes > no){
            cout << "Round " << r << ": Judge says YES" << endl;
            }
            else{
                cout << "Round " << r << ": Judge says NO" << endl;
            }
        }
        // voter
        else {
            // randomly vote yes or no
            int vote = rand() % 2;
            // show the vote
            if(vote == 1) {
                cout << "Round " << r << ": Voter " << id << " votes: YES" << endl;
            }
            else{
                cout << "Round " << r << ": Voter " << id << " votes: NO" << endl;
            }
            // send the vote to the judge through this voter's pipe
            write(fd[id][1], &vote, sizeof(vote));
        }
    }

    // terminate: judge waits for all voters to finish
    if(id == -1){
        for(int i = 0; i < n; i++){
            wait(NULL);
        }
    }
    return 0;
}