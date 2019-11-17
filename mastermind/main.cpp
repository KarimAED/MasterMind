#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>

using namespace std;

struct individual{
    vector<int> guess;
    int score=0;
};

individual pointMut(individual parent, int M)
{
    vector<int>::iterator pit;
    individual temp;

    temp=parent;
    pit=temp.guess.begin()+(rand()%temp.guess.size());
    //cout<<(*it);
    (*pit)=rand()%M;
    return temp;
}

individual swapMut(individual parent)
{
    vector<int>::iterator p1,p2;
    individual temp;
    int temp2;
    temp=parent;
    p1=temp.guess.begin()+(rand()%temp.guess.size());
    do
    {
        p2=temp.guess.begin()+(rand()%temp.guess.size());
    }while (p1==p2);
    temp2=(*p1);
    (*p1)=(*p2);
    (*p2)=temp2;
    return temp;
}

void vecOut(vector<int> Vec)
{
    for (vector<int>::iterator it=Vec.begin();it!=Vec.end();++it)
    {
        cout<<*it<<"\t";
    }
}

class Game
{
public:
    vector<int> Secret;
    int N,M;
    Game(int N, int M);
    int checkSoln(vector<int> Soln);
    void showSecret();
};

Game::Game(int N, int M)
{
    this->N=N;
    this->M=M;
    for (int i=0; i!=N; i++)
    {
        this->Secret.push_back(rand()%M);
    }
}

int Game::checkSoln(vector<int> Soln)
{
    vector<int>::iterator it,it2;
    int score=0,check;

    for (it=this->Secret.begin();it!=this->Secret.end();++it)
    {
        if (*it==Soln.at(it-this->Secret.begin()))
        {
            score+=this->N;
            Soln.at(it-this->Secret.begin())=-1;
        }
        else
        {
            check=0;
            for (it2=Soln.begin();it2!=Soln.end();++it2)
            {
                if (*it==*it2 && !(this->Secret.at(it2-Soln.begin())==*it2) && check==0)
                {
                    score++;
                    *it2=-1;
                    check=1;
                }
            }
        }
    }

    //vecOut(Soln);

    return score;
}

void Game::showSecret()
{
    vecOut(this->Secret);
}





class Solver: public Game
{
public:
    Solver(int N,int M, int popsize);
    void resetPop();
    void showPop();
    void evalPop();
    void sortPop();
    void fillPop();
    void nextGen();
    void breedPop();
    void Evolve();
    vector<individual> population;
    int popsize;
};

Solver::Solver(int N, int M, int popsize):Game(N,M)
{
    this->popsize=popsize;
    this->resetPop();
}

void Solver::resetPop()
{
    this->population.empty();
    this->fillPop();
}

void Solver::fillPop()
{
    int maxIter=(this->popsize-this->population.size());
    for (int c=0;c!=maxIter;c++)
    {
        vector<int> temp;
        individual temp2;
        temp.empty();
        for (int i=0;i!=this->N;i++)
        {
            temp.push_back(rand()%this->M);
        }
        temp2.guess=temp;
        temp2.score=0;
        this->population.push_back(temp2);
    }
}

void Solver::showPop()
{
    cout<<"Current Population: "<<endl;
    for (vector<individual>::iterator it=population.begin();it!=population.end();++it)
    {
        vecOut((*it).guess);
        cout<<"Score: "<<(*it).score<<endl;
    }
}

void Solver::evalPop()
{
    for (vector<individual>::iterator it=(this->population).begin();it!=(this->population).end();++it)
    {
            (*it).score=this->checkSoln((*it).guess);
    }
}

void Solver::sortPop()
{
    bool Swap=true;
    individual temp;
    do
    {
        Swap=false;
        for (vector<individual>::iterator it=this->population.begin();it!=this->population.end()-1;++it)
        {
            if((*it).score<(*(it+1)).score)
            {
                temp=(*(it+1));
                (*(it+1))=(*it);
                (*it)=temp;
                Swap=true;
            }
        }
    }while (Swap==true);
}

void Solver::nextGen()
{
    this->evalPop();
    this->sortPop();
    for (int c=0; c!=this->popsize/2;c++)
    {
        this->population.pop_back();
    }
    this->breedPop();
    this->sortPop();
}


void Solver::breedPop()
{
    vector<individual>::iterator it;
    for(it=this->population.begin();this->population.size()<this->popsize;++it)
    {
        if ((it-this->population.begin())<(this->population.size()/3))
        {
            this->population.push_back(pointMut((*it),this->M));
            this->population.push_back(swapMut(*it));
        }
    }
    this->fillPop();
}

void Solver::Evolve()
{
    int t0=time(NULL);
    int ct=0;
    do
    {
        this->nextGen();
        ct++;
    }while ((this->population.at(0)).score<(this->N*this->N));
    cout<<"Solution found after "<<ct<<" generations and "<<time(NULL)-t0<<" second(s)!"<<endl;
    cout<<"The Solution is: "<<endl;
    vecOut(this->population.at(0).guess);
}

int main()
{
    srand(time(NULL));
    int N, M;
    bool run=true;
    do
    {
    cout<<"\n\n"<<"Welcome to Mastermind!"<<endl;
    cout<<"Enter number of pins to be found: ";
    cin>>N;
    cout<<"Now enter number of colours: ";
    cin>>M;
    if (!(N==-1))
    {
    Solver* game= new Solver(N,M,100);
    cout<<"Secret: "<<endl;
    game->showSecret();
    cout<<"\n\n";
    game->Evolve();
    }
    else
    {
        run=false;
    }
    }while (run);
    return 0;
}
