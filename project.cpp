#include "stdafx.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <cstdlib>
#include <algorithm>
using namespace std;

string format_equation(const string &equation);
float get_coefficient(const string &token);
string get_variable(const string &token);
float get_final_result_value(const string &equation);
vector<string> parse_equation(const string &equation);
vector<string> split(const string &string, char delimiter);
string getpart(int partneeded, string Equation);
float get_coefficient_of_var_needed(string Equation, string VariableName);
string Divided_Equation(string Equation, float VariableName);
string Subtract_Equations(string LHS1, string LHS2, string RHS1, string RHS2);
string Multiply_Equation(string Equation, float multiplyby);
double det2(float Matrix[][100]);
double Determinant(float Matrix[][100], int size);

int main()
{	
	int NumEquations;
	cin >> NumEquations;
	string Equation[100];
	string EqFormated[100];
	string AllVariables[300];
	string command;

	//Get Equations from user as (STRINGS)
	for (int i = 0; i < NumEquations; i++)
	{
		cin >> Equation[i];
	}
	// format the equations in proper form
	for (int n = 0; n < NumEquations; n++)
	{
		EqFormated[n] = format_equation(Equation[n]);

	}
	//count number of variables of all equations (WE WILL USE IT IN CONSTRUCTING MATRICES)
	int numberofvars = 0;
	for (int n = 0; n < NumEquations; n++)
	{
		string avar = EqFormated[n];

		// get variables for specified equation
		for (int i = 0; i < avar.length(); i++)
		{

			string variable = get_variable(getpart(i, avar));
			if (variable.length() == 0) { continue; }
			if (variable[0] == '=') { break; }

			// check of var is already in the array of variables
			int varfound = 0;
			for (int x = 0; x < numberofvars; x++)
			{
				if (AllVariables[x] == variable)
				{
					varfound = 1;
					break;
				}
			}
			// store variable in array of variables if not already exists
			if (varfound == 0) {
				AllVariables[numberofvars] = variable;
				numberofvars++;

			}
		}

	}
	// construct an array for all variable names (WE WILL USE IT IN CONSTRUCTING MATRICES)
	string variables[100]; int counter = 0;
	for (int ne = 0; ne < NumEquations; ne++)
	{
		for (int nv = 0; nv < numberofvars; nv++)
		{

			string variable = get_variable(getpart(nv, EqFormated[ne]));
			bool VariableRepeated = false;
			if (variable[0] == '=') { break; } // go to next eq if we reach the end of current eq rhs
			if (variable == "") { continue; }

			// know if variable already stored in variables array
			for (int i = 0; i < counter; i++)
			{
				if (variable == variables[i]) { VariableRepeated = true; break; }
			}
			if (VariableRepeated == false) // store variable name in array of variables 
			{
				variables[counter] = variable;
				counter++;

			}

		}

	}
	// sort array of variables alphabetically (WE WILL USE IT IN CONSTRUCTING MATRICES)
	for (int i = 0; i < counter; i++)
	{
		for (int j = i + 1; j < counter; j++)
		{

			if (variables[j] < variables[i])
			{
				string temp = variables[i];
				variables[i] = variables[j];
				variables[j] = temp;
			}
		}
	}

	// take command from user and execute it
	while (1)
	{
		getline(cin, command);
		// command number of variables 
		if (command == "num_vars")
		{
			cout << numberofvars << "\n";
		}
		// command print equation 
		if (command.substr(0, command.find_first_of(" ")) == "equation")
		{
			string StNumEqNeeded = command.substr(command.find_first_of(" ") + 1, command.length() - 1);
			int NumOfEqNeeded = atof(StNumEqNeeded.c_str());
			cout << EqFormated[NumOfEqNeeded - 1] << "\n";
		}
		// command coefficients of desired variable
		if (command.substr(0, command.find_first_of(" ")) == "column")
		{
			float coefficientsNeeded[100];
			string DesiredVariable = command.substr(command.find_first_of(" ") + 1, command.length() - command.find_first_of(" ") - 1);
			for (int i = 0; i < NumEquations; i++)
			{   // cout << get_coefficient( '+' + getpart(i, EqFormated[0])) << "\n";
				coefficientsNeeded[i] = get_coefficient_of_var_needed(EqFormated[i], DesiredVariable);
				cout << coefficientsNeeded[i] << "\n";
			}
		}
		// command add equations 
		if (command.substr(0, command.find_first_of(" ")) == "add")
		{
			int indexeq1 = atof(command.substr(4, command.find_last_of(" ") - command.find_first_of(" ")).c_str());
			int indexeq2 = atof(command.substr(command.find_last_of(" ") + 1, command.length() - 1).c_str());
			string LHS1 = EqFormated[indexeq1 - 1].substr(0, EqFormated[indexeq1 - 1].find_first_of("="));
			string LHS2 = EqFormated[indexeq2 - 1].substr(0, EqFormated[indexeq2 - 1].find_first_of("="));
			string LHS = LHS1 + "+" + LHS2;
			string RHS1 = EqFormated[indexeq1 - 1].substr(EqFormated[indexeq1 - 1].find_first_of("=") + 1, EqFormated[indexeq1 - 1].length() - 1);
			string RHS2 = EqFormated[indexeq2 - 1].substr(EqFormated[indexeq2 - 1].find_first_of("=") + 1, EqFormated[indexeq2 - 1].length() - 1);
			string RHS = RHS1 + "+" + RHS2;
			cout << format_equation(LHS + "=" + RHS) << "\n";
		}
		// command subtract equations 
		if (command.substr(0, command.find_first_of(" ")) == "subtract")
		{
			int indexeq1 = atof(command.substr(9, command.find_last_of(" ") - command.find_first_of(" ")).c_str());
			int indexeq2 = atof(command.substr(command.find_last_of(" ") + 1, command.length() - 1).c_str());
			string LHS1 = EqFormated[indexeq1 - 1].substr(0, EqFormated[indexeq1 - 1].find_first_of("="));
			string LHS2 = EqFormated[indexeq2 - 1].substr(0, EqFormated[indexeq2 - 1].find_first_of("="));
			string RHS1 = EqFormated[indexeq1 - 1].substr(EqFormated[indexeq1 - 1].find_first_of("=") + 1, EqFormated[indexeq1 - 1].length() - 1);
			string RHS2 = EqFormated[indexeq2 - 1].substr(EqFormated[indexeq2 - 1].find_first_of("=") + 1, EqFormated[indexeq2 - 1].length() - 1);
			string EquationsMinus = Subtract_Equations(LHS1, LHS2, RHS1, RHS2);
			cout << EquationsMinus << "\n";
		}
		// command substitute
		if (command.substr(0, command.find_first_of(" ")) == "substitute")
		{	/* steps
			2x+2y=2 , 4x+8y=4 , sub x 1 2
			(4x+8y=4)/4 then (x+2y=1)*2 so after this we get 2x+4y=2
			we subtract eq1 - new eq2 555555555555
			*/
			string command6new = command.substr(command.find_first_of(" ") + 1);  // make "substiute x 1 2" >> "x 1 2" and work on new string
			string SubVar = command6new.substr(0, command6new.find_first_of(" ")); // variable name

			int indexeq1 = atof(command6new.substr(command6new.find_first_of(" "), command6new.find_last_of(" ") - command6new.find_first_of(" ")).c_str());
			int indexeq2 = atof(command6new.substr(command6new.find_last_of(" ") + 1, command6new.length() - 1).c_str());

			float dividedby = get_coefficient_of_var_needed(EqFormated[indexeq2 - 1], SubVar); // coeff of subvar in eq2
			string Eq2AfterDiv = Divided_Equation(EqFormated[indexeq2 - 1], dividedby); // divded eq2 with coeff of subvar in eq2 4x+8y=4 > x+2y=1

			float multiplyby = get_coefficient_of_var_needed(EqFormated[indexeq1 - 1], SubVar); // coeff of subvar in eq1
			string Eq2AfterMultiply = Multiply_Equation(Eq2AfterDiv, multiplyby); // multiply eq2 with coeff of subvar in eq1 x+2y=1 > 2x+4y=2

			string Eq2AfterDM = Eq2AfterMultiply;

			string LHS1 = EqFormated[indexeq1 - 1].substr(0, EqFormated[indexeq1 - 1].find_first_of("="));
			string LHS2 = Eq2AfterDM.substr(0, Eq2AfterDM.find_first_of("="));
			string RHS1 = EqFormated[indexeq1 - 1].substr(EqFormated[indexeq1 - 1].find_first_of("=") + 1, EqFormated[indexeq1 - 1].length() - 1);
			string RHS2 = Eq2AfterDM.substr(Eq2AfterDM.find_first_of("=") + 1, Eq2AfterDM.length() - 1);

			string EquationsMinus = Subtract_Equations(LHS1, LHS2, RHS1, RHS2);

			cout << EquationsMinus << endl;
		}
		// command coeff matrix 
		if (command == "D")
		{	// construct coefficients matrix 
			float CoeffMatrix[100][100];
			for (int i = 0; i < NumEquations; i++)
			{
				int IndexOfVar = 0;
				for (int j = 0; j < numberofvars; j++)
				{
					float Coeff = get_coefficient_of_var_needed(EqFormated[i], variables[IndexOfVar]);
					CoeffMatrix[i][j] = Coeff;
					IndexOfVar++;
				}
			}

			// print coefficients matrix 
			for (int i = 0; i < NumEquations; i++)
			{
				for (int j = 0; j < numberofvars; j++)
				{
					cout << CoeffMatrix[i][j] << "\t";
				}
				cout << "\n";
			}
		}
		// command coeff matrix with answer column
		if (command.substr(0, 2) == "D ")
		{
			string VarToReplace = command.substr(command.find_first_of(" ") + 1); // the variable that we will remove it's column 
			string constants[100];

			// construct coefficients matrix (STRING)
			string Matrix[100][100];
			for (int i = 0; i < NumEquations; i++)
			{
				int IndexOfVar = 0;
				for (int j = 0; j < numberofvars; j++)
				{
					ostringstream ss;
					ss << get_coefficient_of_var_needed(EqFormated[i], variables[IndexOfVar]);
					string coeff = ss.str();

					Matrix[i][j] = coeff;
					IndexOfVar++;
				}

			}

			// make array of constants 
			for (int i = 0; i < NumEquations; i++)
			{
				string equation = EqFormated[i];
				constants[i] = equation.substr(equation.find('=') + 1);

			}

			// get index of variable to replace it's column 
			int IndexVarToReplace = 0; // number of column to replace 
			for (int i = 0; i < numberofvars; i++)
			{
				if (VarToReplace == variables[i]) { IndexVarToReplace = i; }
			}

			// replace column of coefficients 
			for (int ne = 0; ne < NumEquations; ne++)
			{
				Matrix[ne][IndexVarToReplace] = constants[ne];
			}

			// print coeff matrix with answer column
			for (int i = 0; i < NumEquations; i++)
			{
				for (int j = 0; j < numberofvars; j++)
				{
					cout << Matrix[i][j] << "\t";
				}
				cout << "\n";
			}
		}
		// command Determinant Value of coefficients matrix
		if (command == "D_value")
		{	// construct coefficients matrix (FLOAT)
			float CoeffMatrix[100][100];
			for (int i = 0; i < NumEquations; i++)
			{
				int IndexOfVar = 0;
				for (int j = 0; j < numberofvars; j++)
				{
					float Coeff = get_coefficient_of_var_needed(EqFormated[i], variables[IndexOfVar]);
					CoeffMatrix[i][j] = Coeff;
					IndexOfVar++;
				}
			}
			// print determinant of coefficients matrix
			cout << Determinant(CoeffMatrix, numberofvars) << endl;
		}
		// commnand solve equations 
		if (command == "solve")
		{
			// construct coefficients matrix (float)
			float CoeffMatrix[100][100];
			for (int i = 0; i < NumEquations; i++)
			{
				int IndexOfVar = 0;
				for (int j = 0; j < numberofvars; j++)
				{
					float Coeff = get_coefficient_of_var_needed(EqFormated[i], variables[IndexOfVar]);
					CoeffMatrix[i][j] = Coeff;
					IndexOfVar++;
				}
			}
			double Delta = Determinant(CoeffMatrix, numberofvars);
			if (Delta == 0 || NumEquations < numberofvars) { cout << "No Solution" << endl; }
			
			else {
				// make array of constants 
				float constants[100];
				for (int i = 0; i < NumEquations; i++)
				{
					string equation = EqFormated[i];
					string constant = equation.substr(equation.find('=') + 1);

					float constantf = atof(constant.c_str());
					constants[i] = constantf;

				}

				// solving for results 
				string results[100];
				for (int solve = 0; solve < numberofvars; solve++)
				{
					float TempMatrix[100][100];
					string variable = variables[solve];
					//construct temporary matrix 
					for (int i = 0; i < numberofvars; i++)
					{
						for (int j = 0; j < numberofvars; j++)
						{
							TempMatrix[i][j] = CoeffMatrix[i][j];
						}
					}

					// replace column of coefficients for current variable
					for (int nEQ = 0; nEQ < NumEquations; nEQ++)
					{
						TempMatrix[nEQ][solve] = constants[nEQ];
					}
					double DeltaVar = Determinant(TempMatrix, numberofvars); // sol = deltavar/delta
					double result = DeltaVar / Delta;

					ostringstream s;
					s << result;
					string ResultString = s.str();

					results[solve] = variable + '=' + ResultString; // array of solutions 
				}
				// print solutions 
				for (int i = 0; i < numberofvars; i++)
				{
					cout << results[i] << endl;
				}

			}
		}

		if (command == "quit") { exit(0); }

	}

	return 0;
}

//function to format equations and get it as (STRING)
string format_equation(const string &equation)
{
	string result;
	map<std::string, float> map;
	std::map<std::string, float>::iterator value;

	// construct the map 
	for (const auto &token : parse_equation(equation))
	{
		float coefficient = get_coefficient(token);
		string variable = get_variable(token);
		if ((value = map.find(variable)) == map.end())
		{
			map[variable] = coefficient;
		}
		else {
			map[variable] = coefficient + value->second;
		}
	}
	// Construct the equation from the map 
	for (value = map.begin(); value != map.end(); ++value)
	{
		string variable = value->first;
		float coefficient = value->second;

		string coefficient2;
		ostringstream coeff;
		coeff << abs(coefficient);
		coefficient2 = coeff.str();

		if (coefficient == 0 || variable.empty()) continue;

		result.append(coefficient > 0 ? "+" : "-"); // add sign

		if (abs(coefficient) != 1) result.append(coefficient2);
		result.append(variable);
	}
	// Append constant of equation
	float constant = ((value = map.find("")) != map.end()) ? -value->second : 0;

	string constant2;
	ostringstream cons;
	cons << constant;
	constant2 = cons.str();
	result.append("=" + constant2);

	// Remove the unnecessary positive sign at the beginning (optional)
	if (result.at(0) == '+') result.replace(0, 1, "");

	return result;
}

// function to get coeff (FLOAT) +3A>>+3 , 
float get_coefficient(const string &token)
{
	int index = token.find_first_not_of(".0123456789+-");
	string coefficient = token.substr(0, index);
	// add 1 beside - or + alone
	if (coefficient.length() == 1)
	{
		coefficient += '1';
	}
	return atof(coefficient.c_str());
}

// function to get variable (STRING)
string get_variable(const string &part)
{
	int index = part.find_first_not_of(".0123456789+-");
	return index < part.length() ? part.substr(index, part.length()) : "";
}

// function to get constant value after '=' (FLOAT)
float get_final_result_value(const string &equation)
{
	int index = equation.find_first_of('=');
	string result = equation.substr(index + 1, equation.length());
	return atof(result.c_str());
}

/*
Input: "20X+12A-3B-15X+B=50"
Output: ["+20X", "+12A", "-3B", "-15X", "+B", "-50"]
*/
vector<string> parse_equation(const string &equation) {
	std::vector<std::string> tokens;

	// Parse tokens including the positive/negative sign
	// All signs after the '=' are flipped
	std::vector<std::string> sides = split(equation, '=');
	for (std::vector<int>::size_type s = 0; s < sides.size(); s++) {
		for (const auto &p : split(sides[s], '+')) {
			std::vector<std::string> parts = split(p, '-');
			for (std::vector<int>::size_type n = 0; n < parts.size(); n++) {
				if (!parts[n].empty()) {
					char sign = n == 0 ? '+' : '-';
					if (s == 1) sign = sign == '+' ? '-' : '+';
					tokens.push_back(sign + parts[n]);
				}
			}
		}
	}

	return tokens;
}

/*
 Input: "a+b+c+d", '+'
 Output: ["a", "b", "c", "d"]
*/
vector<string> split(const string &string, char delimiter) {
	std::string part;
	std::vector<std::string> parts;
	std::stringstream stream(string);
	while (std::getline(stream, part, delimiter)) parts.push_back(part);
	return parts;
}

// function to split equations into parts and get desired part by it's index
string getpart(int partneeded, string Equation)
{
	int Eqlength = Equation.length();
	float startofpart = 0;
	float lengthofpart = 0;
	float p1 = 0;
	int indexofpart = 0;
	string part[500];
	//check each character of equation
	for (int p = 0; p < Eqlength; p++)
	{
		if (Equation[p] == '+' || Equation[p] == '-')
		{
			part[indexofpart] = Equation.substr(p1, lengthofpart);
			p1 = p;
			indexofpart++;
			lengthofpart = 1;
		}

		else { lengthofpart++; }

		if (Equation[p] == '=') {

			part[indexofpart] = Equation.substr(p1, lengthofpart - 1);
			indexofpart++;
			p1 = p;

		}
		part[indexofpart] = Equation.substr(p1, Eqlength - 1);
	}
	return part[partneeded];
}

// function to get coefficent of desired variable
float get_coefficient_of_var_needed(string Equation, string VariableName)
{
	float coefficientNeeded = 0;

	for (int i = 0; i < Equation.length(); i++)
	{
		if (get_variable(getpart(i, Equation)) == VariableName)
		{
			if (i == 0) { coefficientNeeded = get_coefficient('+' + getpart(0, Equation)); return coefficientNeeded; }
			coefficientNeeded = get_coefficient(getpart(i, Equation));
			// coefficientNeeded = get_coefficient(getpart(i, Equation));
			return coefficientNeeded;
		}

	}
	// return string of 0 if variable not found in equation 
	return coefficientNeeded;
}

// function to subtract equations
string Subtract_Equations(string LHS1, string LHS2, string RHS1, string RHS2)
{
	LHS2 = "+" + LHS2;
	//change signs of lhs2
	for (int i = 0; i < LHS2.length(); i++)
	{
		if (LHS2[i] == '+')
		{
			LHS2[i] = '-';
		}
		else if (LHS2[i] == '-')
		{
			LHS2[i] = '+';
		}
	}
	string LHS = LHS1 + "+" + LHS2;  // final form of lhs

									 //change signs of rhs2
	double ConstEq2 = atof(RHS2.c_str());
	ConstEq2 = ConstEq2 * -1;
	ostringstream RHS2Mod;
	RHS2Mod << ConstEq2;
	RHS2 = RHS2Mod.str();

	string RHS = RHS1 + "+" + RHS2;  // final form of rhs
	string EquationsAfterSubtract = format_equation(LHS + "=" + RHS);

	return EquationsAfterSubtract;
}

// function to divivded an equation with coefficient of certain variable
string Divided_Equation(string Equation, float dividedby)
{
	string EqAfterDiv;
	; // we will divided the equation by this number

	map<std::string, float> map;    // map to store equation 
	std::map<std::string, float>::iterator value;

	// Construct Map (Map[x]=3)
	for (const auto &token : parse_equation(Equation))
	{
		float coefficient = get_coefficient(token);
		string variable = get_variable(token);
		map[variable] = coefficient;
	}

	//   Dividing coefficients then construct the equation from the map
	for (value = map.begin(); value != map.end(); ++value) {
		string variable = value->first;
		float CoeffAfterDiv = (value->second) / (dividedby);

		string StrCoeffAfterDiv;
		ostringstream coeff;
		coeff << abs(CoeffAfterDiv);
		StrCoeffAfterDiv = coeff.str();

		if (CoeffAfterDiv == 0 || variable.empty()) continue;

		EqAfterDiv.append(CoeffAfterDiv > 0 ? "+" : "-"); // add sign

		if (abs(CoeffAfterDiv) != 1) EqAfterDiv.append(StrCoeffAfterDiv);
		EqAfterDiv.append(variable);
	}

	// Dividing constant then appending it to equation
	float constant = ((value = map.find("")) != map.end()) ? -value->second / dividedby : 0;

	string StrConstAfterDiv;
	ostringstream cons;
	cons << constant;
	StrConstAfterDiv = cons.str();
	EqAfterDiv.append("=" + StrConstAfterDiv);

	// Remove the unnecessary positive sign at the beginning (optional)
	if (EqAfterDiv.at(0) == '+') EqAfterDiv.replace(0, 1, "");

	return EqAfterDiv;
}

// function to multiply equation 
string Multiply_Equation(string Equation, float multiplyby)
{
	string EquationAfterMultiply;


	map<std::string, float> map;    // map to store equation 
	std::map<std::string, float>::iterator value;

	// Construct Map (Map[x]=3)
	for (const auto &token : parse_equation(Equation))
	{
		float coefficient = get_coefficient(token);
		string variable = get_variable(token);
		map[variable] = coefficient;
	}
	// multiplying equation by a value
	for (value = map.begin(); value != map.end(); ++value) {
		string variable = value->first;
		float CoeffAfterMultiply = (value->second) * (multiplyby);

		string StrCoeffAfterDiv;
		ostringstream coeff;
		coeff << abs(CoeffAfterMultiply);
		StrCoeffAfterDiv = coeff.str();

		if (CoeffAfterMultiply == 0 || variable.empty()) continue;

		EquationAfterMultiply.append(CoeffAfterMultiply > 0 ? "+" : "-"); // add sign

		if (abs(CoeffAfterMultiply) != 1) EquationAfterMultiply.append(StrCoeffAfterDiv);
		EquationAfterMultiply.append(variable);
	}
	// Remove the unnecessary positive sign at the beginning (optional)
	if (EquationAfterMultiply.at(0) == '+') EquationAfterMultiply.replace(0, 1, "");

	bool EquationContainsEqual = false; // if the function gets x+y+z,2 returns 2x+2y+2z without = sign
	for (int i = 0; i < Equation.length(); i++)
	{
		if (Equation[i] == '=')
		{
			EquationContainsEqual = true;
			break;
		}
	}



	// Multiplying constant then appending it to equation

	float constant = ((value = map.find("")) != map.end()) ? -value->second * multiplyby : 0;

	string StrConstAfterDiv;
	ostringstream cons;
	cons << constant;
	StrConstAfterDiv = cons.str();
	EquationAfterMultiply.append("=" + StrConstAfterDiv);

	// if (EquationContainsEqual == false) { return EquationAfterMultiply = EquationAfterMultiply + EquationAfterMultiply.substr(EquationAfterMultiply.find_last_of("+"), 2);; }

	return EquationAfterMultiply;
}

// calculate 2x2 matrix
double det2(float Matrix[][100])
{
	double det2x2 = (Matrix[0][0] * Matrix[1][1]) - (Matrix[0][1] * Matrix[1][0]);
	return det2x2;
}
// function to calculate determinant
double Determinant(float Matrix[][100], int size)
{
	float Minor[100][100];
	double DeterminantValue = 0;
	// intialize minor matrix with zero values to avoid rubbish of memory
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			Minor[i][j] = 0;
		}
	}
	// calculate determinant of 2x2 matrix in last recursive call 
	if (size == 2)
	{
		double det2x2 = det2(Matrix);
		return det2x2;
	}

	// generate minor matrix 
	int IndexOFColumn = 0;
	while (IndexOFColumn < size)
	{
		     int x2 = 0;
			// loop on all equations 
			for (int IndexEquation = 1; IndexEquation < size; IndexEquation++)
			{
				int x1 = IndexEquation;
				int	y2 = 0;
				// loop on all elements of current equation
				for (int IndexVariable = 0; IndexVariable < size; IndexVariable++)
				{
					int y1 = IndexVariable;
					if (y1 == IndexOFColumn) { continue; }

					Minor[x2][y2] = Matrix[x1][y1];

					y2 = y2 + 1; // go to next column of minor matrix
				}
				x2 = x2 + 1; // go to next row of minor matrix
			}
			// size of new minor matrix 
			int newsize = size - 1;
			// sign of column
			int sign = -1;
			if (IndexOFColumn % 2 == 0 || IndexOFColumn == 0) { sign = 1; }
			double constant = Matrix[0][IndexOFColumn];
			double minordetvalue = Determinant(Minor, newsize); // recusive call to with minor matrix to get determinant of it's minor matirx
			DeterminantValue = DeterminantValue + sign * constant*minordetvalue;
		
			IndexOFColumn++;
	}
	return DeterminantValue;
}

