#include <bits/stdc++.h>
#define st first
#define nd second

using namespace std;
ifstream hangmanFile ("hangman.txt");
ifstream wordsFile ("words.txt");

void debug_out() { cerr << endl; }
template<class T> ostream& prnt(ostream& out, T v) { out << v.size() << '\n'; for(auto e : v) out << e << ' '; return out;}
template<class T> ostream& operator<<(ostream& out, vector <T> v) { return prnt(out, v); }
template<class T> ostream& operator<<(ostream& out, set <T> v) { return prnt(out, v); }
template<class T1, class T2> ostream& operator<<(ostream& out, map <T1, T2> v) { return prnt(out, v); }
template<class T1, class T2> ostream& operator<<(ostream& out, pair<T1, T2> p) { return out << '(' << p.st << ' ' << p.nd << ')'; }
template <typename Head, typename... Tail> void debug_out(Head H, Tail... T) { cerr << " " << H; debug_out(T...);}

#define dbg(...) cerr << #__VA_ARGS__ << " ->", debug_out(__VA_ARGS__)
#define dbg_v(x, n) do{cerr<<#x"[]: ";for(int _=0;_<n;++_)cerr<<x[_]<<" ";cerr<<'\n';}while(0)
#define dbg_ok cerr<<"OK!\n"

// const int N = ;
long long n;
string s;
class UserInterface
{
private:
	string hangMan[10];
public: 
	int nbWrongLetters;
	UserInterface()
	{
		int index = 0;
		string aux;
		while (getline (hangmanFile, aux))
		{	
			if (aux == "*")
				index++;
			else
			{
				hangMan[index].append (aux);
				hangMan[index].append ("\n");
			}
		}
		nbWrongLetters = 0;
	}
	void drawHangman()
	{
		cout<<hangMan[nbWrongLetters];
	}
	void clearScreen()
	{
		system("clear");
	}

};
class Game
{
private:
	string word;
	int solvedLetters[100];// 1 pe literele gasite, 0 pe literele negasite
	set<char> wrongLetters;
	set<char> guessedLetters;
	UserInterface ui;
public:
	Game()
	{
		chooseWord();
		initialiseGuessedLetters();
		initialiseSolvedLetters();
	}
	void chooseWord()
	{
		srand (time(NULL));
		string aux;
		int linesNb = count(istreambuf_iterator<char>(wordsFile), istreambuf_iterator<char>(), '\n');
		int randomNb = rand() % linesNb, index = 0;
		// dbg(randomNb, linesNb);
		wordsFile.close();
		wordsFile.open("words.txt");
		while (getline(wordsFile, aux))
		{
			// dbg(randomNb,aux);
			if (index == randomNb)
			{
				word = aux;
				break;
			}
			index ++ ;
		}
	}
	void initialiseGuessedLetters()
	{
		guessedLetters.insert(word[0]);
		guessedLetters.insert(word[word.length() - 1]);
	}
	void initialiseSolvedLetters()
	{
		for (int i = 0; i < word.length(); i++)
		{
			solvedLetters[i] = 0;
			if (guessedLetters.find(word[i]) != guessedLetters.end())
				solvedLetters[i] = 1;
		}
	}
	void printWord()
	{
		for (int i = 0; i < word.length(); i++)
		{
			if (solvedLetters[i] == 1)
			{
				cout<<word[i];
			}
			else cout<<"_";
		}
	}
	int verifyLetter(char character) //returns 1,0,-1, -2-> good letter, bad letter, good letter but already found, bad letter but already tried.
	{
		int ok = 0;
		for (int i = 0; i < word.length(); i++)
		{
			if (word[i] == character)
			{
				if (!ok && guessedLetters.find(word[i]) != guessedLetters.end())
					return -1;

				ok = 1;
				solvedLetters[i] = 1;
			}
		}
		if(wrongLetters.find(character) != wrongLetters.end())
			return -2;
		else 
			return ok;
	}

	void addGuessedLetter(char character)
	{
		guessedLetters.insert(character);
	}

	void addWrongLetter (char character)
	{
		wrongLetters.insert(character);
		ui.nbWrongLetters++;
	}
	int endGame()//returns 0,1,-1 -> !end, win, loss
	{
		// dbg(ui.nbWrongLetters);
		if (ui.nbWrongLetters >= 6)
			return -1;
		for (int i = 0; i < word.length(); i++)
			if (solvedLetters[i] == 0)
				return 0;
		return 1;

	}
	void printLetters(set<char> letters)
	{
		for (set<char>::iterator letter = letters.begin(); letter != letters.end(); letter++)
			cout<<*letter<<' ';

	}
	void gameFlow()
	{
		char character;
		printWord();
		while(!endGame())
		{
			cout<< "\n Ghiciti litera:\n ";
			cin>>character;
			switch (verifyLetter(character)) {
				case 0: 
					addWrongLetter(character);
					ui.clearScreen();
					cout<<"Letter not in the word.\n";
					ui.drawHangman();
					printWord();
					break;
				case 1:
					addGuessedLetter(character);
					cout<<"Good guess!\n";
					ui.clearScreen();
					printWord();
					cout<<"\n";
					break;
				case -1:
					cout<<"good Letter, but already found. Here are the letters that you have guessed.\n";
					printLetters(guessedLetters);
					break;
				case -2:
					cout<<"you have already tried this letter and it is not good.Check out the list below:\n";
					printLetters(wrongLetters);
					break;
			}
		}
		if (endGame() == 1)
			cout<<ui.nbWrongLetters<<"\nFelicitari! ati descoperit cuvantul: "<<word<<endl;
		else { cout<< "\nAi pierdut! Cuvantul era "<<word;}
	}


};
int main() {
	// UserInterface a;
	// a.drawHangman();
	// a.nbWrongLetters++ ;
	// a.drawHangman();
	Game b;
	b.gameFlow();

}	