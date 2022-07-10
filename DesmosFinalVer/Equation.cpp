#include "pch.h"
#include "Equation.h"
#include "Defines.h"


void Equation::Initialize(const int& nodeCount)
{
	size = nodeCount;
	selected = false;
	increaseGraphCount();
	colourVec = { BLACK, BLUE, RED, GREEN, ORANGE,PURPLE,BROWN,MAGENTA, LIGHTGRAY ,GRAY,DARKBLUE, PINK,MAROON,BEIGE, YELLOW,LIME,VIOLET, DARKGRAY ,GOLD,DARKBROWN ,DARKGREEN ,SKYBLUE ,DARKPURPLE };

	SingleChars["tan"] = "a";
	SingleChars["cos"] = "b";
	SingleChars["sin"] = "c";
	SingleChars["acos"] = "d";
	SingleChars["atan"] = "e";
	SingleChars["asin"] = "f";
	SingleChars["csc"] = "g";
	SingleChars["cot"] = "h";
	SingleChars["sec"] = "i";
	SingleChars["sqrt"] = "j";
	SingleChars["abs"] = "k";

	bedmas["-"] = 5;
	bedmas["*"] = 2;
	bedmas["/"] = 2;
	bedmas["a"] = 4;
	bedmas["b"] = 4;
	bedmas["c"] = 4;
	bedmas["d"] = 4;
	bedmas["e"] = 4;
	bedmas["f"] = 4;
	bedmas["g"] = 4;
	bedmas["h"] = 4;
	bedmas["i"] = 4;
	bedmas["j"] = 4;
	bedmas["k"] = 4;
	bedmas["+"] = 1;
	bedmas["^"] = 3;
	bedmas["("] = 0;
}

void Equation::DeInitialize()
{
	SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void Equation::GraphingLogic()
{
	//update current mouse position
	UserMouse = GetMousePosition();

	//Check if the user typed a valid character or pressed delete
	//Add/remove corresponding character from currentInput
	//Use ispressed
	//Print the current string to the screen
	//If the equation is valid, run calculate string on the new string and solve the given vector with different x values stored in the nodes vector
	//If not, draw the error screen (red box, graph unavailable)
	if (IsKeyPressed(KEY_Z) && !selected) increaseGraphCount(); 

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		int index = 0;
		selected = false;

		for (auto& i : textboxes)
		{
			if (CheckCollisionPointRec(UserMouse, i))
			{
				selected = true;
				selectedTextboxIndex = index;
				SetMouseCursor(MOUSE_CURSOR_IBEAM);
			}
			++index;
		}
		if (!selected)
		{
			if (CheckCollisionPointRec(UserMouse, { 0,textboxes[textboxes.size() - 1].y + textboxes[textboxes.size() - 1].height,50 + textboxes[textboxes.size() - 1].width,textboxes[textboxes.size() - 1].height }))
			{
				selected = true;
				increaseGraphCount();
				selectedTextboxIndex = index;
				SetMouseCursor(MOUSE_CURSOR_IBEAM);
			}
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		}
		
		//Check if player clicked outside of the input boxes/wants to stop typing
	}
	if(selected)
	{
		bool stringEdited = false;
		if (IsKeyPressed(KEY_BACKSPACE))
		{
			stringEdited = true;
			if (Ubobs[selectedTextboxIndex].currentInput.size() > 0) Ubobs[selectedTextboxIndex].currentInput.erase(Ubobs[selectedTextboxIndex].currentInput.end() - 1, Ubobs[selectedTextboxIndex].currentInput.end());
		}

		// Get char pressed (unicode character) on the queue
		int key = GetCharPressed();
		if ((key >= 32) && (key <= 125))
		{
			stringEdited = true;
			Ubobs[selectedTextboxIndex].currentInput += key;
		}

		if (stringEdited)
		{
			//Recalculate
			if (ValidateString(Ubobs[selectedTextboxIndex].currentInput))
			{
				Ubobs[selectedTextboxIndex].validDraw = true;
				Ubobs[selectedTextboxIndex].calculatedEq = CalculateString(ParseKeywords(Ubobs[selectedTextboxIndex].currentInput));
				int index=0;
				for (long double i = -((size-1)/2); i <= ((size - 1) / 2); i++)
				{
					nodes[selectedTextboxIndex][index].setX(i*(26/size));
					nodes[selectedTextboxIndex][index].setY(SolveVec(Ubobs[selectedTextboxIndex].calculatedEq, (i * (26 / size))));
					//cout << "\nCreate node: x(" << nodes[index].getX() << ") y(" << nodes[index].getY() << ")\n";
					++index;
				}
			}
			else Ubobs[selectedTextboxIndex].validDraw = false;
		}
	}
}

void Equation::GraphDraw()
{
	//With the nodes vector draw lines between each
	int index = 0, tempColourIndex = 0;
	string drawtext;

	for (auto& i : textboxes)
	{
		//Draw the beginning of the textbox below:
		drawtext = "y = " + Ubobs[index].currentInput;
		DrawText(drawtext.c_str(), (int)i.x + 5, (int)i.y + 16, 20, BLACK);

		//Draw the equation
		if (Ubobs[index].validDraw)
		{
			if (selected) { DrawRectangleLines(0, i.y, i.width, i.height, GREEN); DrawRectangle(0, (int)i.y, 50,(int)i.height, GREEN); }
			else { DrawRectangleLines(0, i.y, i.width, i.height, LIGHTGRAY); DrawRectangle(0, (int)i.y, 50, (int)i.height, LIGHTGRAY);}
		}
		else { DrawRectangleLines(0, i.y, i.width, i.height, RED); DrawRectangle(0, (int)i.y, 50, (int)i.height, RED);}

		drawtext = to_string(index+1) + ":";
		DrawText(drawtext.c_str(), 10, (int)i.y + 16, 20, BLACK);

		if (nodes[index].size() != 0 && Ubobs[index].validDraw)
		{
			for (int i = 1; i < nodes[index].size(); i++)
			{
				long double ypos = scaledYPos(nodes[index][i].getY());
				if (ypos <= screenHeight && ypos >= 0)
				{
					tempColourIndex = index;
					while (index >= colourVec.size()) { tempColourIndex -= colourVec.size(); }
					DrawLineEx({ (float)scaledXPos(nodes[index][i - 1].getX()),(float)scaledYPos(nodes[index][i - 1].getY()) }, { (float)scaledXPos(nodes[index][i].getX()) ,(float)ypos }, 4.f, colourVec[tempColourIndex]);
				}
			}
		}
		++index;
	}

	//Draw the start of the next textbox
	DrawRectangle(0, (int)textboxes[0].y + textboxes.size()*textboxes[0].height, 50, (int)textboxes[0].height, LIGHTGRAY);
	drawtext = to_string(index + 1) + ":";
	DrawText(drawtext.c_str(), 10, textboxes.size() * textboxes[0].height + 16, 20, BLACK);

	////////////////////////////////////////////////////
	//Draw Grids
	DrawLineEx({ (float)scaledXPos(0), 0 }, { (float)scaledXPos(0), screenHeight },5, BLACK);
	DrawLineEx({ 200, (float)scaledYPos(0) }, { screenWidth, (float)scaledYPos(0) }, 5,BLACK);
	for (int i = -15; i <= 15; i++)
	{
		if (i == 0)continue;
		if(scaledXPos(i) > 200 && scaledXPos(i) < screenWidth) DrawLine(scaledXPos(i), 0, scaledXPos(i), screenHeight, BLACK);
		if (scaledYPos(i) > 0 && scaledYPos(i) < screenHeight) DrawLine(200, scaledYPos(i), screenWidth, scaledYPos(i), BLACK);
	}

	for (auto& i : Ubobs)
	{
		//Makes the UserMouse 0 when at beginning of the screen
		UserMouseChanged.x = (UserMouse.x - (200 + ((screenWidth - 200) / 2))) / pointSpacing;
		UserMouseChanged.y = (((screenHeight / 2) - UserMouse.y) / pointSpacing);
		if (i.calculatedEq.size() > 0)
		{
			long double soughtYPos = SolveVec(i.calculatedEq, UserMouseChanged.x);

			if (soughtYPos - 0.5 < UserMouseChanged.y && UserMouseChanged.y < soughtYPos + 0.5)
			{
				string temp1 = to_string(UserMouseChanged.x);
				temp1.erase(temp1.begin() + temp1.find('.') + 3, temp1.end());
				string temp2 = to_string(UserMouseChanged.y);
				temp2.erase(temp2.begin() + temp2.find('.') + 3, temp2.end());

				DrawText(("(" + temp1 + ", " + temp2 + ")").c_str(), UserMouse.x, UserMouse.y,30,BLUE);
			}
			//cout << "X value is: " << UserMouseChanged.x << " sought Y value is: " << SolveVec(i.calculatedEq, UserMouseChanged.x) << " Current Y is: " << UserMouseChanged.y << '\n';
		}
		
	}
}

bool Equation::CheckXY(const string& s)
{
	string temp = s;
	while (true)
	{
		size_t pos = temp.find_first_not_of("y");
		if (pos == string::npos)
		{
			if (temp.size() > 1) return false;
			else break;
		}
		temp.erase(temp.begin() + pos);
	}


	for (short i = 1; i < s.size() - 1; i++)
	{
		if (s[i] == 'x')
		{
			bool front = false, back = false;
			switch (s[i - 1])
			{
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':
				back = true;
				break;
			}
			switch (s[i + 1])
			{
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':
				front = true;
				break;
			}

			if (front && back) return false;
		}
	}
	return true;
}

bool Equation::CheckForCharacters(const string& s)
{
	if (s.find_first_not_of("abceinoqrstx1234567890.()/*-+^ ") != string::npos)
		return false;

	const vector<string> keywords = { "asin","acos","atan","tan","sin","cos","cot","sec","csc","sqrt","abs"};
	string temp = s;
	auto it = temp.begin();
	while (it != temp.end())
	{
		switch (*it)
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '0':
		case '+':
		case '-':
		case 'x':
		case '*':
		case '/':
		case '^':
		case '(':
		case ')':
		case '.':
			it = temp.erase(it);
			break;
		default:
			it++;
			break;
		}
	}

	if (temp.empty()) return true;

	for (const auto& k : keywords)
	{
		size_t pos = temp.find(k);
		while (pos != string::npos)
		{
			temp.erase(temp.begin() + pos, temp.begin() + pos + k.length());
			if (temp.empty()) return true;
			pos = temp.find(k);
		}
	}

	return temp.empty();
}

bool Equation::CheckForDecimalPoints(const string& s)
{
	int flag = 0;
	for (const auto& c : s)
	{
		if (c == '.') ++flag;
		if (flag > 1) break;
	}
	if (flag == 1)
	{
		string temp2 = s.substr(s.find('.'));
		return (temp2.find_first_of("1234567890") != string::npos);
	}

	size_t offset = 0;
	while (true)
	{
		size_t begin = s.find('.', offset);
		if (begin == string::npos) return true;
		size_t end = s.find('.', offset + 1);
		if (end == string::npos)
		{
			string temp1 = s.substr(begin);
			return (temp1.find_first_of("1234567890") != string::npos);
		}
		offset = end;
		if (offset > s.length()) return true;

		string temp = s.substr(begin, end);
		if (temp.find_first_of("/*-+^") == string::npos) return false;
	}
	return true;
}

bool Equation::CheckForOperators(const string& s)
{
	//Check last digit is not an operator
	switch (s[s.length() - 1])
	{
	case '*':
	case '/':
	case '+':
	case '-':
	case '^':
	case '.':
	case 'n':
	case 's':
	case 't':
	case 'c':
		return false;
		break;
	}

	//Check first digit is not an operator
	switch (s[0])
	{
	case '*':
	case '/':
	case '+':
	case '^':
	case '.':
		return false;
		break;
	}

	size_t offset = 0;
	while (true)
	{
		size_t pos = s.find_first_of("/*-+^.", offset);
		if (pos != string::npos)
		{
			offset = pos + 1;
			switch (s[offset])
			{
			case '*':
			case '/':
			case '+':
			case '^':
			case '.':
				return false;
				break;
			}
		}
		else break;
	}
	return true;
}

bool Equation::CheckForBrackets(const string& s)
{
	int openBrackets = 0;
	for (const auto& c : s)
	{
		if (c == '(') ++openBrackets;
		if (c == ')') --openBrackets;
	}
	if (openBrackets != 0) return false;
	return openBrackets == 0;
}

bool Equation::ValidateString(string& s)
{
	if (s.size() == 0) return false;
	std::replace(s.begin(), s.end(), ',', '.');
	s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
	if (!CheckForBrackets(s)) return false;
	if (!CheckForOperators(s)) return false;
	if (!CheckForDecimalPoints(s)) return false;
	if (!CheckForCharacters(s))	return false;
	if (!CheckXY(s)) return false;
	return true;
}

vector<string> Equation::CalculateString(string s)
{
	numberStack.clear();
	s.erase(remove(s.begin(), s.end(), ' '), s.end());
	bool wasnum = true;
	//BEDMAS brackets exponents division multiplication addition subtraction
	string temp = "";
	for (int i = 0; i < s.length(); i++)
	{
		wasnum = true;
		for (const auto& symbol : bedmas)
		{
			if (symbol.first[0] == s[i])
			{
				wasnum = false;
				if (temp != "")
				{
					numberStack.push_back(temp);
					temp = "";
				}
				if (operationStack.size() == 0)
				{
					operationStack.push_back(string(1, s[i]));
				}
				else
				{
					if (s[i] == '(')
					{
						operationStack.push_back(string(1, s[i]));
					}
					else
					{
						while (!operationStack.empty() && bedmas[operationStack[operationStack.size() - 1]] > bedmas[string(1, s[i])])
						{
							numberStack.push_back(operationStack[operationStack.size() - 1]);
							operationStack.pop_back();
						}
						operationStack.push_back(string(1, s[i]));
					}
				}
			}
		}

		if (s[i] == ')')
		{
			wasnum = false;
			if (temp != "")
			{
				numberStack.push_back(temp);
				temp = "";
			}
			while (!operationStack.empty() && operationStack[operationStack.size() - 1] != "(")
			{
				numberStack.push_back(operationStack[operationStack.size() - 1]);
				operationStack.pop_back();
			}
			if (!operationStack.empty()) operationStack.pop_back();
		}
		else if (wasnum) {
			temp += string(1, s[i]);
		}
	}
	if (temp != "") {
		numberStack.push_back(temp);
	}
	for (int i = operationStack.size() - 1; i >= 0; i--)
	{
		numberStack.push_back(operationStack[i]);
	}
	operationStack.clear();
	
	cout << "Calculate string returned: ";
	for (auto& i : numberStack) { cout << i; }
	cout << '\n';
	return numberStack;
}

long double Equation::SolveVec(vector<string> vec, long double x)
{
	for (auto& i : vec)
		if (i == "x") i = to_string(x);
	long double answer = 0;
	auto it = vec.begin();

	while (it != vec.end())
	{
		size_t foundIndex = it->find_first_of("/*^+-@abcdefghijk");
		if (foundIndex != string::npos)
		{
			string sizeCheck = *it;
			if (sizeCheck.size() > 1) { ++it; }
			else if (*it == "-" || * it == "a" || *it == "b" || *it == "c" || *it == "d" || *it == "e" || *it == "f" || *it == "g" || *it == "h" || *it == "i"|| *it == "j" || *it == "k")
			{
				auto it2 = it;
				long double a = stold(*it2.operator--());
				string temp = *it;
				switch (temp[0])
				{
				case 'a':
					temp = to_string(tangent(a));
					break;
				case 'b':
					temp = to_string(cosine(a));
					break;
				case 'c':
					temp = to_string(sine(a));
					break;
				case 'd':
					temp = to_string(arccosine(a));
					break;
				case 'e':
					temp = to_string(arctangent(a));
					break;
				case 'f':
					temp = to_string(arcsine(a));
					break;
				case 'g':
					temp = to_string(cosecant(a));
					break;
				case 'h':
					temp = to_string(cotangent(a));
					break;
				case 'i':
					temp = to_string(secant(a));
					break;
				case 'j':
					temp = to_string(sqrt(a));
					break;
				case 'k':
					temp = to_string(a>=0 ? a : -a);
					break;
				case '-':
					temp = to_string(negativefy(a));
					break;
				}

				it2 = it;
				*it2.operator--() = temp;
				it = vec.erase(it, it + 1);
			}
			else
			{
				auto it2 = it;
				long double a = stold(*it2.operator-=(2));
				it2 = it;
				long double b = stold(*it2.operator--());

				string temp = *it;
				switch (temp[0])
				{
				case '+':
					temp = to_string(addition(a,b));
					break;
				case '/':
					temp = to_string(divide(a,b));
					break;
				case '*':
					temp = to_string(multiply(a,b));
					break;
				case '^':
					temp = to_string(power(a,b));
					break;
				}
				it2 = it;
				*it2.operator-=(2) = temp;
				it = vec.erase(it - 1, it + 1);
			}
			///////////////////////
			////TESTING
			//cout << "\nCurrent vector is: ";
			//for (auto& i : vec)
			//{
			//	cout << i << ", ";
			//}
			//cout << std::endl;
			////////////////////////
		}
		else ++it;
	}
	return stold(vec[0]);
}

string Equation::ParseKeywords(string returnValue)
{
	//Remove spaces
	returnValue.erase(remove(returnValue.begin(), returnValue.end(), ' '), returnValue.end());

	//Make sure to check asin before sin since sin is a substring of asin but not vice versa
	const vector<string> keywords = { "asin","acos","atan","tan","sin","cos","cot","sec","csc","sqrt","abs"};

	//Replace all keywords with @symbols
	for (short i = 0; i < keywords.size(); i++)
	{
		size_t a = returnValue.find(keywords[i]);
		if (a != string::npos)
		{
			returnValue[a] = SingleChars[keywords[i]][0];
			returnValue.erase(returnValue.begin() + a + 1, returnValue.begin() + a + keywords[i].length());
		}
	}
	//Add bracket multiplications
	//Opening brackets
	size_t pos = returnValue.find('(');
	while (pos != string::npos)
	{
		char c;
		if (pos > 0)
		{
			c = returnValue[pos - 1];
			if ((c >= '0' && c <= '9') || c == 'x' || c == ')')
			{
				returnValue.insert(returnValue.begin() + pos, '*');
			}
		}
		pos = returnValue.find('(', pos + 1);
	}

	//Closing brackets
	pos = returnValue.find(')');
	while (pos != string::npos)
	{
		char c;
		if (pos < returnValue.size()-1)
		{
			c = returnValue[pos + 1];
			if ((c >= '0' && c <= '9') || c == 'x')
			{
				returnValue.insert(returnValue.begin() + pos+1, '*');
			}
		}
		pos = returnValue.find(')', pos + 1);
	}

	//Check if subtraction signs are negatives or operators
	pos = returnValue.find('-');
	while (pos != string::npos)
	{
		char c;

		if (pos > 0)
		{
			c = returnValue[pos - 1];
			if (pos != 0 && (c == ')' || (c >= '0' && c<= '9')))
			{
				returnValue.insert(returnValue.begin() + pos, '+');
			}
		}
		pos = returnValue.find('-', pos + 2);
	}

	//Check if there is a number or a function before or after x
	pos = returnValue.find('x');
	while (pos != string::npos)
	{
		char c;
		if (pos < returnValue.size() - 1)
		{
			//Check digits next to x
			c = returnValue[pos + 1];
			//Check if stuff next to x is a number or a method
			if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'i') || c == 'x')
			{
				returnValue.insert(returnValue.begin() + pos + 1, '*');
			}
		}
		if (pos > 0)
		{
			c = returnValue[pos - 1];
			//Check if stuff next to x is a number or a method
			if ((c >= '0' && c <= '9') || c == 'x')
			{
				returnValue.insert(returnValue.begin() + pos, '*');
			}
		}
		pos = returnValue.find('x', pos + 1);
	}



	return returnValue;
}

void Equation::increaseGraphCount()
{
	vector<GraphNode> tempNodes;
	for (long double i = -(size / 2); i < (size / 2); i++)
	{
		tempNodes.push_back(GraphNode(i, 0));
	}
	cout << std::endl;
	nodes.push_back(tempNodes);

	User newGraph;
	Ubobs.push_back(newGraph);

	Rectangle newTextbox;
	if (textboxes.size() > 0) newTextbox = { 50,textboxes[textboxes.size() - 1].y + 50,200,50 };
	else newTextbox = { 50, 0.f, 200.f, 50.f };
	textboxes.push_back(newTextbox);
}
