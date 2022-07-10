#pragma once
#include "SymbolDefines.h"
#include "Clicker.h"


struct User
{
	string currentInput = "";
	vector<string> calculatedEq = {};
	bool validDraw = false;
};

class GraphNode
{
private:
	long double y;
	long double x;
public:
	GraphNode() :y(0), x(0) {}
	GraphNode(const long double& num1, const long double& num2) : y(num2), x(num1){}

	const long double& getX() const { return x; } 
	const long double& getY() const { return y; }
	void setX(const long double& num) { x = num; }
	void setY(const long double& num) { y = num; }

	inline const Vector2 getVec2() const { return { (float) x,(float) y }; }
};


class Equation
{
public:
	static void Initialize(const int& nodeCount);
	static void DeInitialize();
	static void GraphingLogic();
	static void GraphDraw();
	static bool ValidateString(string& s);
	static vector<string> CalculateString(string s);
	static long double SolveVec(vector<string> vec, long double x);
	static string ParseKeywords(string returnValue);

private:
	inline static int selectedTextboxIndex;
	inline static long double size;
	inline static bool selected;
	inline static Vector2 UserMouse = GetMousePosition();
	inline static Vector2 UserMouseChanged;

#pragma region operatorFunctions
	inline static long double d2r(const long double& a) { return a * (PI / 180); }
	inline static long double scaledXPos(const long double& x) {return 200 + ((screenWidth - 200) / 2) + x * pointSpacing;}
	inline static long double scaledYPos(const long double& y){return (screenHeight / 2) - y * pointSpacing; }
	inline static long double multiply(const long double& a, const long double& b) { return a * b; }
	inline static long double divide(const long double& a, const long double& b) { return a / b; }
	inline static long double addition(const long double& a, const long double& b) { return a + b; }
	inline static long double negativefy(const long double& a) { return -a; }
	inline static long double power(const long double& a, const long double& b) { return pow(a, b); }
	
	inline static long double tangent(const long double& a) { return tan(a); }
	inline static long double cosine(const long double& a) { return cos(a); }
	inline static long double sine(const long double& a) { return sin(a); }
	inline static long double arccosine(const long double& a) { return acos(a); }
	inline static long double arcsine(const long double& a) { return asin(a); }
	inline static long double arctangent(const long double& a) { return atan(a); }
	inline static long double cotangent(const long double& a) { return 1 / (tan(a)); }
	inline static long double cosecant(const long double& a) { return 1 / (cos(a)); }
	inline static long double secant(const long double& a) { return 1 / (sin(a)); }

#pragma endregion
#pragma region containers
	inline static unordered_map<string, int> bedmas = {};
	inline static vector<string> operationStack = {};
	inline static vector<string> numberStack = {};
	inline static vector<vector<GraphNode>> nodes;
	inline static vector<Rectangle> textboxes;
	inline static vector<User> Ubobs;
	inline static unordered_map<string, string> SingleChars = {};
	inline static vector<Color> colourVec;
#pragma endregion
	static bool CheckXY(const string& s);
	static bool CheckForCharacters(const string& s);
	static bool CheckForDecimalPoints(const string& s);
	static bool CheckForOperators(const string& s);
	static bool CheckForBrackets(const string& s);

	static void increaseGraphCount();
};

