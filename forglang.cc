#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

class Command
{
private:
   char command;
   int data;
   int line;

public:
   Command(char c, int d, int l) : command{c}, data{d}, line{l} {};
   operator string()
   {
      string out;
      out += to_string(line + 1);
      if (line + 1 < 10)
      {
         out += " ";
      }
      if (line + 1 < 100)
      {
         out += " ";
      }
      out += "| ";
      out += command;
      out += to_string(data);
      out += '\n';
      return out;
   }
   char get_command() { return command; }
   int get_data() { return data; }
   int get_line() { return line; }
   int get_jump()
   {
      if (line % 2 == 0)
      {
         return line / 2;
      }
      else
      {
         return 1 + line * 3;
      }
   }
};

class OutOfBoundsException
{
private:
   int endLine;

public:
   OutOfBoundsException(int endLine) : endLine{endLine} {}
   int getLine() { return endLine; }
};

class Forg
{
private:
   vector<Command> map;
   int line;
   std::map<int, int> memory;
   vector<int> path;

public:
   Forg(vector<Command> map) : map{map}, line{1} { path.push_back(line + 1); };
   Forg(vector<Command> map, int line) : map{map}, line{line} { path.push_back(line + 1); };
   void execute()
   {
      switch (map[line].get_command())
      {
      case '*':
         while (!(cin >> memory[map[line].get_data()]))
         {
            cin.clear();
            cin.ignore(1);
         }
         break;
      case '^':
         cout << "Forg says '" << memory[map[line].get_data()] << "'" << endl;
         break;
      case '+':
         memory[map[line].get_data()]++;
         break;
      case '-':
         memory[map[line].get_data()]--;
         break;
      default:
         break;
      }
   }
   void jump()
   {
      // cout << "From " << line + 1 << endl;
      if ((line + 1) % 2 == 0)
      {
         line = (line + 1) / 2 - 1;
         // cout << "Jumped to " << line + 1 << endl;
         swim(-1);
      }
      else
      {
         line = 3 * (line + 1);
         // cout << "Jumped to " << line + 1 << endl;
         swim(1);
      }
      // cout << "Swam to " << line + 1 << endl;
      path.push_back(line + 1);
   }
   void swim(int dir)
   {
      if (line < 0 || line >= map.size() || map[line].get_command() == 'z')
      {
         path.push_back(line + 1);
         throw OutOfBoundsException{line};
      }

      while (map[line].get_command() == '~' || (map[line].get_command() == 'd' && memory[map[line].get_data()] == 0) || (map[line].get_command() == 'p' && memory[map[line].get_data()] != 0))
      {
         line += dir;
         if (line < 0 || line >= map.size() || map[line].get_command() == 'z')
         {
            path.push_back(line + 1);
            throw OutOfBoundsException{line};
         }
      }
   }
   vector<int> getPath()
   {
      return path;
   }
   std::map<int, int> getMemory(){
      return memory;
   }
};

vector<Command> parseFile(istream *input_stream)
{
   vector<Command> out;
   stringstream in;
   stringstream in2;
   char c;
   while (true)
   {
      *input_stream >> c;
      if ((*input_stream).eof())
         break;
      in << c;
      in2 << c;
   }
   in.clear();
   in2.clear();

   char cdump;
   char ctest;
   int i;
   int idump;

   in2 >> cdump;
   while (true)
   {
      // Read command
      in >> c;

      // Test if next char is a number, if so read it
      in2 >> ctest;
      if (isdigit(ctest))
      {
         in >> i;
         while (isdigit(ctest) && !in2.eof())
         {
            in2 >> ctest;
         }
      }
      else
      {
         i = 0;
      }
      out.push_back(Command(c, i, out.size()));

      if (in2.eof())
      {
         break;
      }
   }
   return out;
}

int main(int argc, char const *argv[])
{
   vector<Command> commands;
   ifstream program;
   bool debugMode = false;

   for (int i = 0; i < argc; i++)
   {
      string arg = argv[i];
      if (arg == "-d")
      {
         debugMode = true;
      }
      if (arg == "--help" || arg == "/?" || arg == "-h" || arg == "-?")
      {
         cout << "Usage: " << argv[0] << " program [-d]" << endl
              << "Options:" << endl
              << "\t -d \tDebug mode" << endl;
         return 0;
      }
   }

   if (argc <= 1)
   {
      cout << "No input file specified" << endl
           << "Type '" << argv[0] << " --help' for usage";
      return 0;
   }
   program.open(argv[1]);
   if (!program)
   {
      cout << "Could not find file '" << argv[1] << "'" << endl;
      return 0;
   }
   else
   {
      commands = parseFile(&program);
   }

   cout << "-- Running " << argv[1] << (debugMode ? " [Debug mode]" : "") << " --" << endl;

   int forg_start = 0;

   for (auto c : commands)
   {
      if (c.get_command() == '@')
      {
         forg_start = c.get_line();
         break;
      }
   }

   Forg f(commands, forg_start);

   while (true)
   {
      try
      {
         f.execute();
         f.jump();
      }
      catch (OutOfBoundsException)
      {
         cout << endl
              << "Done!" << endl;
         break;
      }
   }

   if (debugMode)
   {
      cout << endl << "-- Debug --" << endl;
      cout << "Path: ";
      for (auto i : f.getPath())
      {
         cout << i << "->";
      }
      cout << "END" << endl;
      cout << "Final memory dump: ";
      for (auto i : f.getMemory()){
         cout << i.first << "[" << i.second << "] ";
      }
      cout << endl;
   }

   return 0;
}