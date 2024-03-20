#include <iostream>
#include <vector>

using namespace std;

class LcgGenerator{
private:
    uint16_t mCurrentX, mM,mA,mC;
public:
    LcgGenerator(uint16_t mA, uint16_t mC, uint16_t mCurrentX, uint16_t mM){
        this->mM=mM;
        this->mC=mC;
        this->mCurrentX=mCurrentX;
        this->mA=mA;
    }
    uint16_t next(){
        mCurrentX=(mA*mCurrentX+mC)%mM;
        return mCurrentX;
    }
};
class Card{
private:
    uint16_t mValue;
    string mSuite;
public:
    Card(string tip_carte, uint16_t valoare){
        mSuite=tip_carte;
        mValue=valoare;
    }
    bool operator==(const Card & carte) const{
        if(mSuite==carte.mSuite && mValue==carte.mValue) return true;
        return false;
    }
    string toString() const {
        string valoare = to_string(mValue) + "," + mSuite;
        return valoare;
    }
    uint16_t getBlackjackValue()const{
        if(mValue<=10) return mValue;
        else return 10;
    }
    bool isFake() const{
        if(mSuite=="trefla" || mSuite=="negru" || mSuite=="rosu" || mSuite=="romb")
        {
            return true;
        }
        if(!(mValue>=2 && mValue<=14)) return true;
        return false;
    }

};
class CardPack{
private:
    vector<Card> mCards;
    uint16_t mCurrentIndex;
public:
    CardPack(bool empty){
        if(empty==true) {
            mCurrentIndex=0;
            mCards.clear();
        }
        else
        {
            mCurrentIndex=0;
            uint16_t i,j;
            for(i=0;i<4;i++)
            {
                for(j=2;j<=14;j++)
                {
                    if(j!=11)
                    {
                        if(i==0)
                            mCards.push_back(Card("rosu",j));

                        if(i==1)
                            mCards.push_back( Card("negru",j));


                        if(i==2)
                            mCards.push_back(Card("romb",j));

                        if(i==3)
                            mCards.push_back( Card("trefla",j));
                    }
                }
            }
            mCurrentIndex=0;
        }
    }
    bool goodForBackjack() const{
        for(uint32_t i=0;i<mCards.size();i++){
            if(mCards[i].isFake()){
                return false;
            }
        }
        return true;
    }
    bool ok() const{
        if(goodForBackjack()){
            if(mCards.size()==52){
                for(uint32_t i=0;i<52;i++){
                    for(uint32_t j=0;j<52;j++)
                    {

                        if(j!=i && mCards[j]==mCards[i]) return false;
                    }
                }
            }
            else return false;
        }
        return true;
    }
    void addCard(const Card & carte){
        mCards.push_back(carte);
    }
    void shuffle(uint16_t a1 ,uint16_t c1 , uint16_t x01 , uint16_t a2 ,uint16_t c2 ,uint16_t x02 ) {
        mCurrentIndex = 0;
        LcgGenerator first(a1, c1, x01, mCards.size());
        LcgGenerator second(a2, c2, x02, mCards.size());
        for (int i = 0; i < 50; i++) {

            uint16_t val1 = first.next();
            uint16_t val2 = second.next();
            swap(mCards[val1], mCards[val2]);
        }
    }
    void printPack() const{
        for(int32_t i=0;i<mCards.size();i++){
            cout<<mCards[i].toString()<<endl;
        }
    }
    Card nextCard(){
        return mCards[mCurrentIndex++];
    }
};
class Player{
private:
    uint16_t mPoints;
    string mName;
    uint16_t mMoney;
public:
    Player(string nume, uint16_t bani){
        mName=nume;
        mMoney=bani;
        mPoints=0;
    }
    string toString()const{
        string val = mName+": "+ to_string(mMoney);
        return val;
    }
    void pick(const Card & carte){
        mPoints=mPoints+carte.getBlackjackValue();
    }
    void play(CardPack &pachet){
        while(mPoints<17){
            pick(pachet.nextCard());
        }
    }
    void newRound(){
        mPoints=0;
    }
    bool lostRound()const{
        if(mPoints>21) return true;
        return false;
    }
    uint16_t getPoints()const{
        return mPoints;
    }
    void operator--(){
        mMoney-=10;
    }
    void operator++(){
        mMoney+=10;
    }
    bool lostGame()const{
        if(mMoney<10) return true;
        return false;
    }

};



int main(){
    CardPack pachet(true);
    vector<Player> jucatori;
    string command,stema;
    int nr;
    cin>>command;
    if(command=="check_cards"){
        while(cin>>nr){
            getchar();
            cin>>stema;
            Card carte(stema,nr);
            pachet.addCard(carte);
        }
        int check1=0,check2=0;
        if(pachet.goodForBackjack()) check1=1;
        if(pachet.ok()) check2=1;
        if(check1==1 && check2==1) cout<<"Pachet OK\n";
        if(check1==1 &&check2==0) cout<<"Pregatit pentru Blackjack\n";
        if(check1==0) cout<<"Pachet masluit\n";
    }
    if(command=="shuffle_cards"){
        uint16_t a1, c1, x01, a2, c2, x02;
        cin>>a1>>c1>>x01>>a2>>c2>>x02;
        while(cin>>nr){
            getchar();
            cin>>stema;
            Card carte(stema,nr);
            pachet.addCard(carte);
        }
        pachet.shuffle(a1, c1, x01, a2, c2, x02);
        pachet.printPack();
    }
    string pname="";
    uint16_t nr_players=0;
    uint16_t pbani=0;;
    Player dealer("dealer",2933);
    if(command=="play_game"){
        CardPack pachet(false);
        cin>>nr_players;
        for(uint32_t i=0;i<nr_players;i++)
        {
            cin>>pname>>pbani;
            Player jucator(pname,pbani);
            jucatori.push_back(jucator);

        }
        uint16_t a0=0,c0=0,x0=0,a1=0,c1=0,x1=0;
        while(cin>>a0>>c0>>x0>>a1>>c1>>x1)
        {
            pachet.shuffle(a0,c0,x0,a1,c1,x1);
            for(uint32_t i=0;i<jucatori.size();i++){
                jucatori[i].pick(pachet.nextCard());
                jucatori[i].pick(pachet.nextCard());
            }
            dealer.pick(pachet.nextCard());
            dealer.pick(pachet.nextCard());

            for(uint32_t i=0;i<jucatori.size();i++){
                jucatori[i].play(pachet);
            }
            dealer.play(pachet);
            for(uint64_t i=0;i<jucatori.size();i++)
            {
                if(!jucatori[i].lostRound() && (dealer.getPoints() < jucatori[i].getPoints() || dealer.lostRound()))
                {
                    jucatori[i].operator++();
                }
                if((!dealer.lostRound() && dealer.getPoints() > jucatori[i].getPoints()) || jucatori[i].lostRound() )
                {
                    jucatori[i].operator--();
                }
                if(!jucatori[i].lostGame())
                {
                    jucatori[i].newRound();

                }
                else
                {
                    jucatori.erase(jucatori.begin()+i);
                    i--;
                }
            }
            dealer.newRound();
        }
        for(Player p  : jucatori){
            cout<<p.toString()<<"\n";
        }
    }
    return 0;
}