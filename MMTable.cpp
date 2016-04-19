#include "MMTable.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <istream>
#include <sstream>

using namespace std;

MMTable::MMTable()
{
    //ctor
    for(int i = 0; i<tableSize; i++) {          //empty record initialize
        hashTable[i] = NULL;
    }
}

MMTable::~MMTable()
{
    //dtor
}


void MMTable::printTableContents() {
    TeamElem *temp;
    int counter = 0;
    int num = 0;
    cout<<"Teams Eligible for March Madness, by hash index:"<<endl;
    for(int i = 0; i<tableSize; i++) {
        temp = hashTable[i];
        while (temp != NULL) {
            num++;
            cout<<num<<": "<<temp->team<<", "<<temp->state<<endl;
            temp = temp->next;
            counter++;
        }
    }
    if(!counter) cout<<"empty"<<endl;
    return;
}

void MMTable::printTeamNames() {
  //sort table in a array/vector of type TeamElem alphabetically -> can we use assignment 1 stuff for this?
  //print vector/array
  cout<<"In print team names function"<<endl;
}


void MMTable::printTop25Ranking() {
  //sort table in a array/vector of type TeamElem in order by overall rank from 1-25, -1s at the bottom
  //need a variable to keep track of element ranking just put in
  //print vector/array
    cout<<"In print top 25 function"<<endl;
}


void MMTable::printRegionalRanking() {
  //sort table in a array/vector of type TeamElem in order by regional rank
  //There should be 4 of each number, so we would put team into array if it was equal to or +1 from the previous data?
  //print vector/array
    cout<<"In print regional rankings function"<<endl;
}


void MMTable::insertTeam(string team, string state, int top25_ranking, string region, int region_rank) {
    int index = hashSum(team, tableSize);
    if(hashTable[index] == NULL) {
        hashTable[index] = new TeamElem(team, state, top25_ranking, region, region_rank);
    } else {
        TeamElem *temp = hashTable[index];
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new TeamElem(team, state, top25_ranking, region, region_rank);
        temp->next->previous = temp;
    }
    return;
}


void MMTable::deleteTeam(string team) {
    int index = hashSum(team, tableSize);
    TeamElem *temp = hashTable[index];
    while (temp != NULL) {
        if (temp->team == team) {
            if(temp->previous != NULL) {                //if not the 1st element in an index row
                temp->previous->next = temp->next;
                if(temp->next != NULL)                  //if not the last element in an index row
                    temp->next->previous = temp->previous;
            }
            // First element in index row
            else {
                if (temp->next == NULL) {               // the first and last element in an index row
                    hashTable[index] = temp->next;
                } else {
                    hashTable[index] = temp->next;
                    temp->next->previous = NULL;
                }
            }
            return;
        }
        temp = temp->next;
    }
    delete temp;
    cout << "Delete Team: Team not found in March Madness Database" <<endl;
    cout<<endl;
    return;
}

TeamElem *MMTable::findTeam(string name) {
    int index = hashSum(name, tableSize);
    TeamElem *temp = hashTable[index];
    while (temp != NULL) {
        if (temp->team == name) {
            cout<< temp->region<<" #"<<temp->region_rank <<": " << temp->team <<", "<< temp->state <<endl;
            cout<<"Games won so far: "<<temp->games_won<<", "<<"Games lost so far: "<<temp->games_lost<<", "<<"Games tied so far: "<<temp->games_tied<<endl;
            if(temp->top25_rank > 0) {
              cout<<"Overall Rank: "<<temp->top25_rank<<endl;
            } else {
              cout<<"Not ranked in Top 25"<<endl;
            }
            cout<<endl;
            return temp;
        }
        temp = temp->next;
    }
    cout << "Find Team: Team not found in March Madness Database" <<endl;
    cout<<endl;
    return temp;
}


void MMTable::compareTeams(string name1, string name2) {
    cout<<endl;
    cout<<"TEAM 1: "<<endl;
    TeamElem *team1 = findTeam(name1);
    cout<<"vs."<<endl;
    cout<<"TEAM 2: "<<endl;
    TeamElem *team2 = findTeam(name2);
    cout<<endl;

    if(team1 != NULL && team2 != NULL) {
        if((team1->top25_rank > team2->top25_rank) || (team1->region_rank > team2->region_rank)) {
          cout<<team1->team<<" likely to beat "<<team2->team<<endl;
        } else if((team2->top25_rank > team1->top25_rank) || (team2->region_rank > team1->region_rank)) {
          cout<<team1->team<<" likely to beat "<<team2->team<<endl;
        } else {
          cout<<team1->team<<" and "<<team2->team<<" likely to tie."<<endl;
        }
    } else {
      cout<<"One or more teams not found in March Madness Database" <<endl;
    }
    cout<<endl;
    return;
}


void MMTable::playTeams(string name1, string name2) {
    TeamElem *team1 = findTeam(name1);
    TeamElem *team2 = findTeam(name2);

    if(team1 != NULL && team2 != NULL) {
          if((team1->top25_rank > 0 && team2->top25_rank > 0) || team1->region == team2->region) {
              if((team1->top25_rank > team2->top25_rank) || (team1->region_rank > team2->region_rank)) {
                cout<<team1->team<<" beats "<<team2->team<<endl;
                team1->games_won++;
                team2->games_lost++;
              } else if((team2->top25_rank > team1->top25_rank) || (team2->region_rank > team1->region_rank)) {
                cout<<team2->team<<" beats "<<team1->team<<endl;
                team2->games_won++;
                team1->games_lost++;
              } else {
                cout<<team1->team<<" and "<<team2->team<<" tie."<<endl;
                team2->games_tied++;
                team1->games_tied++;
              }
          } else {
            cout<<"Teams must be in Top 25 or in same bracket region to play each other."<<endl;
          }
    }
    cout<<endl;
    return;
}



void MMTable::findCommonStates(string name) {
    //Traverse hash table until finding a match to input team name
    int index = hashSum(name, tableSize);
    TeamElem *team1 = hashTable[index];
    while (team1 != NULL) {
        if (team1->team == name)
            break;
        team1 = team1->next;
    }
    if(team1 == NULL) {
        cout << "Team not found in March Madness Database" <<endl;
        return;
    } else {
        // Find matching teams with common state
        TeamElem *team2;
        int counter = 0;
        int teams_in_state = 1;
        cout<<team1->state<<":"<<endl;
        cout<<team1<<endl;

        for(int i = 0; i<tableSize; i++) {
            team2 = hashTable[i];
            while (team2 != NULL) {
                if(team2->state == team1->state) {
                  cout<<team2->team<<endl;
                  teams_in_state++;
                }
                team2 = team2->next;
                counter++;
            }
        }
        cout<<team1->state<<" has "<<teams_in_state<<" teams in March Madness"<<endl;
        cout<<endl;
        return;
  }
}


int MMTable::hashSum(string key, int table_size) {
    int sum = 0;
    for(int i = 0; i<key.size(); i++)
        sum += key[i];
    sum %= table_size;
    return sum;
}
