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
};

class Forg
{
private:
   vector<Command> map;
   int line;
   std::map<int, int> memory;

public:
   Forg(vector<Command> map) : map{map}, line{1} {};
   Forg(vector<Command> map, int line) : map{map}, line{line} {};
   void execute()
   {
      switch (map[line].get_command()){
         case '*':
            cin >> memory[map[line].get_data()];
            break;
         case '^':
            cout << memory[map[line].get_data()] << endl;
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
   }
   void swim(int dir)
   {
      if (line < 0 || line >= map.size() || map[line].get_command() == 'z')
      {
         throw OutOfBoundsException{};
      }

      while (map[line].get_command() == '~' || (map[line].get_command() == 'd' && memory[map[line].get_data()] == 0) || (map[line].get_command() == 'p' && memory[map[line].get_data()] != 0))
      {
         line += dir;
         if (line < 0 || line >= map.size() || map[line].get_command() == 'z')
         {
            throw OutOfBoundsException{};
         }
      }
   }
};

vector<Command> parse(istream *input_stream)
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
   if (argc == 2)
      commands = parse(new ifstream(argv[1]));
   else
   {
      cout << "No input file specified" << endl;
      return 0;
   }

   cout << "Running " << argv[1] << endl;

   int forgl = 0;

   for (auto c : commands)
   {
      if (c.get_command() == '@')
      {
         forgl = c.get_line();
         break;
      }
   }

   Forg f(commands, forgl);

   while (true)
   {
      try
      {
         f.execute();
         f.jump();
      }
      catch (OutOfBoundsException)
      {
         cout << "Done!" << endl;
         break;
      }
   }

   return 0;
}