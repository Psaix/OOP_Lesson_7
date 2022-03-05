#include <iostream>
#include <vector>
#include <algorithm>
#include <string>


class Card
{
public:
    enum rank {
        ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
        JACK, QUEEN, KING
    };
    enum suit { CLUBS, DIAMONDS, HEARTS, SPADES };

    Card(rank r = ACE, suit s = SPADES, bool ifu = true);
    int GetValue() const;
    void Flip();

private:
    rank m_Rank;
    suit m_Suit;
    bool m_IsFaceUp;

    friend std::ostream& operator<<(std::ostream& os, const Card& aCard);
};


std::ostream& operator<<(std::ostream& os, const Card& aCard)
{
    const std::string RANKS[] = { "0", "A", "2", "3", "4", "5", "6", "7", "8", "9","10", "J", "Q", "K" };
    const std::string SUITS[] = { "c", "d", "h", "s" };

    if (aCard.m_IsFaceUp)
    {
        os << RANKS[aCard.m_Rank] << SUITS[aCard.m_Suit];
    }
    else
    {
        os << "XX";
    }

    return os;
}



Card::Card(rank r, suit s, bool ifu) : m_Rank(r), m_Suit(s), m_IsFaceUp(ifu)
{}

int Card::GetValue() const
{
    int value = 0;
    if (m_IsFaceUp)
    {
        value = m_Rank;
        if (value > 10)
        {
            value = 10;
        }
    }
    return value;
}

void Card::Flip()
{
    m_IsFaceUp = !(m_IsFaceUp);
}


class Hand
{
public:
    Hand();

    virtual ~Hand();

    void Add(Card* pCard);

    void Clear();

    int GetTotal() const;

protected:
    std::vector<Card*> m_Cards;
};

Hand::Hand()
{
    m_Cards.reserve(7);
}

Hand::~Hand()
{
    Clear();
}

void Hand::Add(Card* pCard)
{
    m_Cards.push_back(pCard);
}

void Hand::Clear()
{
    std::vector<Card*>::iterator iter = m_Cards.begin();
    for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
    {
        delete* iter;
        *iter = 0;
    }

    m_Cards.clear();
}

int Hand::GetTotal() const
{
    if (m_Cards.empty())
    {
        return 0;
    }

    if (m_Cards[0]->GetValue() == 0)
    {
        return 0;
    }

    int total = 0;
    std::vector<Card*>::const_iterator iter;
    for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
    {
        total += (*iter)->GetValue();
    }

    bool containsAce = false;
    for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
    {
        if ((*iter)->GetValue() == Card::ACE)
        {
            containsAce = true;
        }
    }

    if (containsAce && total <= 11)
    {

        total += 10;
    }

    return total;
}


class GenericPlayer : public Hand
{
    friend std::ostream& operator<<(std::ostream& os, const GenericPlayer& aGenericPlayer);

public:
    GenericPlayer(const std::string& name = "");

    virtual ~GenericPlayer();

    virtual bool IsHitting() const = 0;

    bool IsBoosted() const;

    void Bust() const;

protected:
    std::string m_Name;
};


bool GenericPlayer::IsBoosted() const
{
    return (GetTotal() > 21);
}


void GenericPlayer::Bust() const
{
    std::cout << m_Name << " busts.\n";
}


std::ostream& operator<<(std::ostream& os, const GenericPlayer& aGenericPlayer)
{
    os << aGenericPlayer.m_Name << ":\t";

    std::vector<Card*>::const_iterator pCard;
    if (!aGenericPlayer.m_Cards.empty())
    {
        for (pCard = aGenericPlayer.m_Cards.begin();
            pCard != aGenericPlayer.m_Cards.end();
            ++pCard)
        {
            os << (*pCard) << "\t";
        }


        if (aGenericPlayer.GetTotal() != 0)
        {
            std::cout << "(" << aGenericPlayer.GetTotal() << ")";
        }
    }
    else
    {
        os << "<empty>";
    }

    return os;
}



class Player : public GenericPlayer
{
public:
    Player(const std::string& name = "");

    virtual ~Player();

    virtual bool IsHitting() const;

    void Win() const;

    void Lose() const;

    void Push() const;
};


bool Player::IsHitting() const
{
    std::cout << m_Name << ", do you want a hit? (Y/N): ";
    char response;
    std::cin >> response;
    return (response == 'y' || response == 'Y');
}


void Player::Win() const
{
    std::cout << m_Name << " wins.\n";
}

void Player::Lose() const
{
    std::cout << m_Name << " loses.\n";
}

void Player::Push() const
{
    std::cout << m_Name << " pushes.\n";
}




class House : public GenericPlayer
{
public:
    House(const std::string& name = "House");

    virtual ~House();

    virtual bool IsHitting() const;

    void FlipFirstCard();
};

bool House::IsHitting() const
{
    return (GetTotal() <= 16);
}


void House::FlipFirstCard()
{
    if (!(m_Cards.empty()))
    {
        m_Cards[0]->Flip();
    }
    else
    {
        std::cout << "No card to flip!\n";
    }
}


//____________________________________________________________________________________________________________
//____________________________________________TASK3___________________________________________________________
//____________________________________________________________________________________________________________


class Deck : public Hand
{
public:
    Deck();

    virtual ~Deck();

    void Populate();

    void Shuffle();

    void Deal(Hand& aHand);

    void AdditionalCards(GenericPlayer& aGenericPlayer);
};

Deck::Deck()
{
    m_Cards.reserve(52);
    Populate();
}

Deck::~Deck()
{}

void Deck::Populate()
{
    Clear();

    for (int s = Card::CLUBS; s <= Card::SPADES; ++s)
    {
        for (int r = Card::ACE; r <= Card::KING; ++r)
        {
            Add(new Card(static_cast<Card::rank>(r),
                static_cast<Card::suit>(s)));
        }
    }
}

void Deck::Shuffle()
{
    std::random_shuffle(m_Cards.begin(), m_Cards.end());
}

void Deck::Deal(Hand& aHand)
{
    if (!m_Cards.empty())
    {
        aHand.Add(m_Cards.back());
        m_Cards.pop_back();
    }
    else
    {
        std::cout << "Out of cards. Unable to deal.";
    }
}


void Deck::AdditionalCards(GenericPlayer& aGenericPlayer)
{
    std::cout << std::endl;

    while (!(aGenericPlayer.IsBoosted()) && aGenericPlayer.IsHitting())
    {
        Deal(aGenericPlayer);
        std::cout << aGenericPlayer << std::endl;

        if (aGenericPlayer.IsBoosted())
        {
            aGenericPlayer.Bust();
        }
    }
}


//____________________________________________________________________________________________________________
//____________________________________________TASK4___________________________________________________________
//____________________________________________________________________________________________________________



class Game
{
public:
    Game(const std::vector<std::string>& names);

    ~Game();

    void Play();

private:
    Deck m_Deck;
    House m_House;
    std::vector<Player> m_Players;
};

Game::Game(const std::vector<std::string>& names)
{
    std::vector<std::string>::const_iterator pName;
    for (pName = names.begin(); pName != names.end(); ++pName)
    {
        m_Players.push_back(Player(*pName));
    }

    srand(static_cast<unsigned int>(time(0)));
    m_Deck.Populate();
    m_Deck.Shuffle();
}

Game::~Game()
{}

void Game::Play()
{
    std::vector<Player>::iterator pPlayer;
    for (int i = 0; i < 2; ++i)
    {
        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
        {
            m_Deck.Deal(*pPlayer);
        }
        m_Deck.Deal(m_House);
    }

    m_House.FlipFirstCard();

    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
    {
        std::cout << *pPlayer << std::endl;
    }
    std::cout << m_House << std::endl;

    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
    {
        m_Deck.AdditionalCards(*pPlayer);
    }

    m_House.FlipFirstCard();
    std::cout << std::endl << m_House;

    m_Deck.AdditionalCards(m_House);

    if (m_House.IsBoosted())
    {
        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
        {
            if (!(pPlayer->IsBoosted()))
            {
                pPlayer->Win();
            }
        }
    }
    else
    {
        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end();
            ++pPlayer)
        {
            if (!(pPlayer->IsBoosted()))
            {
                if (pPlayer->GetTotal() > m_House.GetTotal())
                {
                    pPlayer->Win();
                }
                else if (pPlayer->GetTotal() < m_House.GetTotal())
                {
                    pPlayer->Lose();
                }
                else
                {
                    pPlayer->Push();
                }
            }
        }

    }

    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
    {
        pPlayer->Clear();
    }
    m_House.Clear();
}

//____________________________________________________________________________________________________________
//____________________________________________TASK5___________________________________________________________
//____________________________________________________________________________________________________________

std::vector<std::string> getPlNames();

int main()
{
    std::cout << "\t\tWelcome to Blackjack!\n\n";
    std::vector<std::string> names = getPlNames();
    std::cout << '\n';

    Game aGame(names);
    char again = 'y';
    while (again != 'n' && again != 'N')
    {
        aGame.Play();
        std::cout << "\n Do you want to play again? (Y/N):\n";
        std::cin >> again;
    }
    return 0;
}


std::vector<std::string> getPlNames()
{
    int numPlayers = 0;
    while (numPlayers < 1 || numPlayers > 7)
    {
        std::cout << "How many players? (1-7):\n";
        std::cin >> numPlayers;
    }
    std::vector<std::string> names(numPlayers);
    std::string name;
    for (int i = 0; i < numPlayers; ++i)
    {
        std::cout << "Enter name for player " << i + 1 << ": ";
        std::cin >> name;
        names.at(i) = name;
    }
    return names;
}
