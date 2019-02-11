// Funktion, parses message and extracts value from it.
// first two characters are ignored.
// Parameter 1: complete String message
// Parameter 2: identifier: "pe" expected for now
#include<iostream>
#include<string>
#include<sstream>
#include<arduino.h>


// ----------------------------------------------------------------------------------------------

int getValueFromMessage(std::string msg, std::string id)
{
    // convert string message number to value through substring
    
    if (id == "pe")
    {
        // Zahl hat string-length Stellen minus 2 (pe)
        std::string substring = msg.substr(2, (msg.length() - 2));
        
        //return std::stoi(substring); //only works for compilers supporting C++11
        //alternative for older compilers:
        int x(0);
        // object from the class stringstream:
        std::stringstream geek(substring);
        // stream the value of substring to integer x:
        geek >> x;
        return x;
    }
    
    return -1;
}
// ----------------------------------------------------------------------------------------------

int percentageToNumberOfLeds(int value)
{   // in case 4 LED be used
    return static_cast<int>(value / 100.0 * 4);
}
// ----------------------------------------------------------------------------------------------

void lightProcessLEDs(int numberOfLeds, int ledPin1, int ledPin2, int ledPin3, int ledPin4)
{
    // converts the input integer numberOfLeds to the actual High / Low of the according Pins
    // to create a process bar containing 4 LEDs
    if(numberOfLeds == 1)
    {   // >=25%
        digitalWrite(ledPin1, HIGH);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);
        digitalWrite(ledPin4, LOW);
    }
    else if(numberOfLeds == 2)
    {   // >=50%
        digitalWrite(ledPin1, HIGH);
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin3, LOW);
        digitalWrite(ledPin4, LOW);
    }
    else if(numberOfLeds == 3)
    {   // >=75%
        digitalWrite(ledPin1, HIGH);
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin3, HIGH);
        digitalWrite(ledPin4, LOW);
    }
    else if(numberOfLeds == 4)
    {   // 100%
        digitalWrite(ledPin1, HIGH);
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin3, HIGH);
        digitalWrite(ledPin4, HIGH);
    }
    else
    {   // <25%
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);
        digitalWrite(ledPin4, LOW);
    }
}
// ----------------------------------------------------------------------------------------------
