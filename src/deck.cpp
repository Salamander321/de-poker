#include "deck.h"

Deck::Deck()
{
    m_initialized = false;
    init();
}

Deck::~Deck()
{
	for (int i = 0; i < DECKSIZE; i++)
		delete m_cards[i];
}

void Deck::init()
{
    if(m_initialized) return;
    for(int i = 0; i < SUITSIZE; i++)
    {
        for(int j = 0; j < CARDVALUESIZE; j++)
        {
            m_cards[i * CARDVALUESIZE + j] = new Card(Card::SUIT(i), Card::VALUE(j));
        }
    }
    m_topCardIndex = 0;
    m_initialized = true;
}

Card* Deck::popCard()
{
    Card *pop = m_cards[m_topCardIndex++];
    return pop;
}

void Deck::burnCard()
{ 
	++m_topCardIndex; 
}

void Deck::getFlop(Card* &c1, Card* &c2, Card* &c3)
{
    burnCard();
    c1 = m_cards[m_topCardIndex++];
    c2 = m_cards[m_topCardIndex++];
    c3 = m_cards[m_topCardIndex++];
}

Card* Deck::getTurn()
{
    burnCard();
    return popCard();
}

Card* Deck::getRiver()
{
    burnCard();
    return popCard();
}

void Deck::quickShuffle()
{
    // mersenne twister random number generator
    std::mt19937 randomGen(static_cast<unsigned int> (std::time(nullptr)));     
    std::uniform_int_distribution<> randomCard(0, DECKSIZE - 1);

    for(int i = 0; i < DECKSIZE; i++)
    {
        swapCards(randomCard(randomGen), randomCard(randomGen));
    }
}

void Deck::fisherYatesShuffle()
{
	std::mt19937 randomGen(static_cast<unsigned int>(std::time(nullptr)));
	std::uniform_int_distribution<> randomCard(0, DECKSIZE);

	for (int i = DECKSIZE - 1; i > 0; i--)
	{
		randomCard.param(std::uniform_int_distribution<>::param_type(0, i));
		swapCards(randomCard(randomGen), i);
	}
}

void Deck::swapCards(int card1, int card2)
{
    Card *temp_card = m_cards[card1];
    m_cards[card1] = m_cards[card2];
    m_cards[card2] = temp_card;
}

void Deck::display()
{
	for (int i = m_topCardIndex + 1; i <= DECKSIZE; i++)
	{
		std::cout << m_cards[i-1]->getSuit() << 
			", " << m_cards[i-1]->getValue() << '\t';
		if (i % CARDVALUESIZE == 0)
			std::cout << std::endl;
	}
}