#pragma once

#include "Types.hpp"
#include <stdio.h>
#include <string>

class TextReader 
{
public:
    enum TokenResult 
    {
        Number = 0,
        String = 1,
        End = 2,
        Error = 3
    };

public:
    TextReader(const char* Path) :
        File(null)
    {
        fopen_s(&this->File, Path, "r");
    }

    ~TextReader()
    {
        if (this->File)
        {
            fclose(this->File);
            this->File = null;
        }
    }

public:
    TokenResult GetToken()
    {
        if (!this->File)
        {
            return End;
        }

	    char Letter;

        while (true) 
        {
            if ((Letter = (char)(fgetc(this->File))) == EOF)
            {
                return End;
            }

            if (isspace((ui8)(Letter)) != 0)
            {
                continue;
            }

            if (Letter == '/' || Letter == '#')
            {
                while (true)
                {
                    if ((Letter = (char)(fgetc(this->File))) == EOF)
                    {
                        return End;
                    }

                    if (Letter == '\n')
                    {
                        break;
                    }
                }

                if (Letter != '\n')
                {
                    break;
                }
            }
        }

        i32 Count = -1;

        switch (Letter)
        {
            case '-':
            case '.':
            case '*':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            {
                ungetc(Letter, this->File);

                while ((Letter = (char)(fgetc(this->File))) != EOF && (Letter == '-' || Letter == '.' || Letter == '*' || isdigit((ui8)(Letter)) != 0))
                {
                    this->Buffer[++Count] = Letter;
                }
                
                this->Buffer[++Count] = 0;

                return Number;
            }
            case '"':
            {
                while ((Letter = (char)(fgetc(this->File))) != EOF && Letter != '"')
                {
                    this->Buffer[++Count] = Letter;
                }
                
                this->Buffer[++Count] = 0;

                if (Letter != '"')
                {
                    ungetc(Letter, this->File);
                }
                
                return String;
            }
            default:
            {
                if (isalpha((ui8)(Letter)) != 0)
                {
                    while ((Letter = (char)(fgetc(this->File))) != EOF && (Letter == '.' || Letter == '_' || isalnum((ui8)(Letter)) != 0))
                    {
                        this->Buffer[++Count] = Letter;
                    }
                    
                    this->Buffer[++Count] = 0;

                    ungetc(Letter, this->File);

                    return String;
                }

                break;
            }
        }
        
        return Error;
    }

    template<typename T>
    T Get() const
    {
        if (strcmp(this->Buffer, "*") == 0)
        {
            return (T)(-1);
        }

        return (T)(atoi(this->Buffer));
    }

    template<>
    std::string Get() const
    {
        return (std::string)(this->Buffer);
    }

    template<>
    const char* Get() const
    {
        return this->Buffer;
    }

    template<>
    float Get() const
    {
        if (strcmp(this->Buffer, "*") == 0)
        {
            return -1.f;
        }

        return (float)(atof(this->Buffer));
    }

    template<>
    double Get() const
    {
        if (strcmp(this->Buffer, "*") == 0)
        {
            return -1.0;
        }

        return atof(this->Buffer);
    }

    template<>
    i64 Get() const
    {
        if (strcmp(this->Buffer, "*") == 0)
        {
            return (i64)(-1);
        }

        return _atoi64(this->Buffer);
    }

    template<>
    ui64 Get() const
    {
        if (strcmp(this->Buffer, "*") == 0)
        {
            return (ui64)(-1);
        }

        return (ui64)(_atoi64(this->Buffer));
    }

    template<>
    bool Get() const
    {
        if (strcmp(this->Buffer, "*") == 0)
        {
            return false;
        }

        return atoi(this->Buffer) == 1;
    }

private:
    FILE* File;
    char  Buffer[256];
};