#include <iostream>
using namespace std;
#include <cstdlib> 
#include <string>
#include <ctime> 
#include <cmath>
#include <ios>
#include <iomanip>

string suit[] ={"S","H","D","C"};
int value[]={11,2,3,4,5,6,7,8,9,10,10,10,10};
int rank[]={1,2,3,4,5,6,7,8,9,10,11,12,13};


//structure for the Cards
struct Card{
  string suit;
  string info;
  int rank;
  int value;

  Card(){
    suit=info="";
    rank=0;
    value=0;
  }
};

//structure for the card array
struct CardArray{
  int maxCard;
  int currentCard;
  Card* temp;
  Card*playerHand;
  Card*dealerHand;
  CardArray(){
    temp=playerHand=dealerHand=NULL;
    maxCard=0;
    currentCard=0;
  }
};

//function prototypes
void getNewDeck(CardArray & deck);
void printDeck(const CardArray & deck);
void shuffleDeck(CardArray & deck);
int blackjack(CardArray & deck);
int dealerDeal(CardArray & deck, int cardInhand);
int playerDeal(CardArray & deck, int cardInhand);
void revealPlayer(CardArray & deck, int playerCard);
void revealDealer(CardArray & deck, int dealerCard, bool hide);
int HitStand(CardArray &deck, int playerCard);
int dealingToDealer(CardArray & deck, int dealerCard);
int valueCalc(CardArray & deck, int numberOfCard, bool dealer);
bool WinOrBust(CardArray & deck, int playervalue, int dealervalue, int dealerCard);
void endWinner(CardArray & deck, int playervalue, int dealervalue);
bool repeatGame();

int main() {

  CardArray deck;//initalize a CardArray variable
  getNewDeck(deck);//create deck
  shuffleDeck(deck);//shuffle deck
  printDeck(deck);//print deck

  bool repeat=true;
  while (repeat==true){
    blackjack(deck);//blackjack game

    repeat=repeatGame();//let user choose to repeat the game
  }
} 

//create new 52 card deck
void getNewDeck(CardArray & deck){
  const int CARDS=52;//52 cards in a deck
  int numberRank=0;

  deck.maxCard= deck.currentCard= CARDS;//assigning starting value to the number of cards
  deck.temp = new Card[CARDS];

  //assigning each card and their infos to their array
  for(int iteration=0; iteration<4; iteration++){
    for (int i=1; i<=13; i++){
      //storing the suit of the card
      if(iteration==0){
        deck.temp[numberRank].suit="S";
      }

      else if(iteration==1){
        deck.temp[numberRank].suit="H";
      }

      else if(iteration==2){
        deck.temp[numberRank].suit="D";
      }

      else if(iteration==3){
        deck.temp[numberRank].suit="C";
      }

      //the rank of card
      deck.temp[numberRank].rank=i;


      //stroing the value of card
      if (i==1){
        deck.temp[numberRank].value=11;
      }

      else if (i>10){
        deck.temp[numberRank].value=10;
      }

      else{
        deck.temp[numberRank].value=i;
      }


      //storing the info of card
      string stringvalue= to_string(i);
      if(i==1){
        deck.temp[numberRank].info= "A" + deck.temp[numberRank].suit;
      }
      else if(i==11){
        deck.temp[numberRank].info= "J" + deck.temp[numberRank].suit;
      }
      else if(i==12){
        deck.temp[numberRank].info= "Q" + deck.temp[numberRank].suit;
      }
      else if(i==13){
        deck.temp[numberRank].info= "K" + deck.temp[numberRank].suit;
      }
      else {
        deck.temp[numberRank].info= stringvalue + deck.temp[numberRank].suit;
      }

      numberRank++;//rank of the card +1
    }
  } 
}

//print the deck
void printDeck(const CardArray & deck){
  //print cards in order by their suits
  for (int i=0; i<13; i++){
    cout<<deck.temp[i].info<<" ";
  }
  cout<<endl;
  for (int i=13; i<26; i++){
    cout<< deck.temp[i].info<<" ";
  }
  cout<<endl;
  for (int i=26; i<39; i++){
    cout<< deck.temp[i].info<<" ";
  }
  cout<<endl;
  for (int i=39; i<52; i++){
    cout<< deck.temp[i].info<<" ";
  }

}

//shuffle the deck
void shuffleDeck(CardArray & deck){
  srand(time(nullptr));
  //swap the cards radomly for 52 times
  for(int i=0; i<52;i++){
    int random=rand()%52;
    Card swap= deck.temp[i];
    deck.temp[i]=deck.temp[random];
    deck.temp[random]=swap;
  }
}

//blackjack game function
int blackjack(CardArray & deck){
  //create empty arrays
  deck.playerHand = new Card[12];
  deck.dealerHand = new Card[12];
  //the number of cards in player's and dealer's hands
  int playerCard=0;
  int dealerCard=0;
  int playervalue=0;
  int dealervalue=0;


  //deal the first card
  cout<<"\nDEAL FIRST CARD" <<endl <<"--------------"<<endl; 
  playerCard=playerDeal(deck, playerCard);
  dealerCard=dealerDeal(deck, dealerCard);
  revealPlayer(deck,playerCard);
  revealDealer(deck,dealerCard,0);

  //deal the second card  
  cout<<"\nDEAL SECOND CARD" <<endl <<"--------------"<<endl;
  playerCard=playerDeal(deck, playerCard);
  dealerCard=dealerDeal(deck, dealerCard);
  revealPlayer(deck,playerCard);
  revealDealer(deck,dealerCard,1);

  playervalue = valueCalc(deck, playerCard, 0);
  dealervalue= valueCalc(deck, dealerCard,1);
  
  int endProgram = WinOrBust(deck, playervalue,dealervalue, dealerCard);
  if (endProgram==1){
    return 0;
  }

  //card dealing to the player
  cout<<"DEALING TO PLAYER"<<endl<<"--------------"<<endl;
  playerCard = HitStand(deck, playerCard);
  endProgram=playerCard;

  if (endProgram==1){
    return 0;
  }


  cout<<"\nDEALING TO DEALER"<<endl<<"--------------"<<endl;
  dealerCard= dealingToDealer(deck, dealerCard);
  endProgram= dealerCard;
  if (endProgram==1){
    return 0;
  }


  playervalue = valueCalc(deck, playerCard, 0);
  dealervalue= valueCalc(deck, dealerCard,1);
  cout<<endl;
  endWinner(deck,playervalue,dealervalue);

  return 0;
}


int playerDeal(CardArray & deck, int cardInhand){
  int current=deck.currentCard-1;
  deck.playerHand[cardInhand].info= deck.temp[current].info;
  deck.playerHand[cardInhand].value= deck.temp[current].value;
  cardInhand++;
  deck.currentCard--;
  return cardInhand;
}


int dealerDeal(CardArray & deck, int cardInhand){
  int current=deck.currentCard-1;
  deck.dealerHand[cardInhand].info= deck.temp[current].info;
  deck.dealerHand[cardInhand].value= deck.temp[current].value;
  cardInhand++;
  deck.currentCard--;
  return cardInhand;
}


void revealPlayer(CardArray & deck, int playerCard){
  cout<<"+Player+ ";
  for(int i=0; i<playerCard; i++){
    cout<< deck.playerHand[i].info<<" ";
  }
  cout<<endl;
}

void revealDealer(CardArray & deck, int dealerCard, bool hide){
  cout<<"+Dealer+ ";
  for(int i=0; i<dealerCard; i++){
    if (dealerCard==2 && i==1 && hide==true){
      cout<< "?? "<<endl;
    }
    else{
      cout<< deck.dealerHand[i].info<<" ";

      if(i+1==dealerCard){
        cout<<endl;
      }

    }
  }
}

int HitStand(CardArray & deck, int playerCard){
  int playervalue=0;
  string hitOrStand="";
  while(hitOrStand!="s" && hitOrStand!="S"){
    cout<< "Enter h to hit or s to stand: ";
    cin>>hitOrStand;

    if (hitOrStand=="h" || hitOrStand=="H"){
      playerCard=playerDeal(deck, playerCard);
      revealPlayer(deck,playerCard);
      playervalue= valueCalc(deck, playerCard, 0);
      int endProgram= WinOrBust(deck, playervalue, 0, 2);

      if (endProgram==1){
        return 1;
      }
    }
  }

  return playerCard;

}

int dealingToDealer(CardArray & deck, int dealerCard){
  int dealervalue=0;
  int random= 0; 
  revealDealer(deck,dealerCard,0);
  dealervalue=valueCalc(deck, dealerCard, 1);

  while (dealervalue<12){
    random=rand()%50+1;

    if (random==50){
      return dealerCard;
    }

    else{
      dealerCard=dealerDeal(deck, dealerCard);
      revealDealer(deck,dealerCard,0);
      dealervalue=valueCalc(deck, dealerCard, 1);
      int endProgram= WinOrBust(deck, 0, dealervalue, dealerCard);

      if (endProgram==1){
        return 1;
      }

    }

  }

  while (dealervalue<15){
    random=rand()%3+1;
    
    if (random==1){
      return dealerCard;
    }

    else{
      dealerCard=dealerDeal(deck, dealerCard);
      revealDealer(deck,dealerCard,0);
      dealervalue=valueCalc(deck, dealerCard, 1);
      int endProgram= WinOrBust(deck, 0, dealervalue, dealerCard);

      if (endProgram==1){
        return 1;
      }

    }

  }

   while (dealervalue<17){
    random=rand()%4+1;
    
    if (random!=1){
      return dealerCard;
    }

    else{
      dealerCard=dealerDeal(deck, dealerCard);
      revealDealer(deck,dealerCard,0);
      dealervalue=valueCalc(deck, dealerCard, 1);
      int endProgram= WinOrBust(deck, 0, dealervalue, dealerCard);

      if (endProgram==1){
        return 1;
      }

    }

  }

  while (dealervalue<20){
    random=rand()%50+1;
    
    if (random!=1){
      return dealerCard;
    }

    else{
      dealerCard=dealerDeal(deck, dealerCard);
      revealDealer(deck,dealerCard,0);
      dealervalue=valueCalc(deck, dealerCard, 1);
      int endProgram= WinOrBust(deck, 0, dealervalue, dealerCard);

      if (endProgram==1){
        return 1;
      }

    }

  }

    if(dealervalue==20){
     return dealerCard;
    }
  return 0;
}

int valueCalc(CardArray & deck, int numberOfCard, bool dealer){
  int value=0;

  if (dealer==1){
    for (int i=0; i<numberOfCard; i++){
      value+= deck.dealerHand[i].value;
    }
    return value;
  }


  if (dealer==0){
    for (int i=0; i<numberOfCard; i++){
      value+= deck.playerHand[i].value;
    }
    return value;
  }

  return 0;
}


bool WinOrBust(CardArray & deck, int playervalue, int dealervalue, int dealerCard){

  if (playervalue>21){
    revealDealer(deck, dealerCard ,0);
    cout<<"busted!!! You lost"<<endl;
    return 1;
  }

  else if (dealervalue>21){
    cout<<"Dealer busted!!! You win" << endl;
    return 1;
  }

  if (playervalue==21 && dealervalue==21){
    revealDealer(deck, dealerCard ,0);
    cout<<"Two Blackjack !!! TIE"<<endl;
    return 1;
  }

  else if (playervalue==21){
    revealDealer(deck, dealerCard ,0);
    cout<<"BlackJack, You win"<<endl;
    return 1;
  }

  else if (dealervalue==21){
    if (dealerCard==2){
      revealDealer(deck, dealerCard ,0);
    }
    cout<<"Dealer blackjack, you lost"<<endl;
    return 1;
  }

  return 0;
}

void endWinner(CardArray & deck, int playervalue, int dealervalue){
  if (playervalue>dealervalue){
    cout << "Player value= "<<playervalue<< " Dealer value= "<< dealervalue <<endl;
    cout<<"Player Win!!!";
  }

  if (playervalue<dealervalue){
    cout << "Player value= "<<playervalue<< " Dealer value= "<< dealervalue <<endl;
    cout<<"Dealer Win!!!";
  }

  if (playervalue==dealervalue){
    cout << "Player value= "<<playervalue<< " Dealer value= "<< dealervalue <<endl;
    cout<<"Game Tied!!!";
  }
  cout<<endl;
}

bool repeatGame(){
  string choice;
  cout<<"Do you want to play another hand (y to play)?";
  cin>>choice;
  if (choice=="y" || choice=="Y"){
    return 1;
  }

  else{
    return 0;
  }
}