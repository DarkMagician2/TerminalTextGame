//capital or lowercase w, a, s, or d - movement capitals for convenience when placing down objects
//ijkl - use current item in that direction
//backspace - disable/enable collision
//1-225 then space - select item
//periods are empty spaces in inventory
//current item:1 - delete
//current item:/ - pickup
//current item:. - empty
//capital I, J, K, or L - place currently selected item
//x - delete one of current held item
//capital X - delete all of current held item
//: - start typing a command
//space - execute command
//:give - type item wanted immediately after writing this, then hit space to run it
//:multgive - give but you type a 3 digit amount after multgive before the item

#include <iostream>
#include <vector>
#include <regex>
#include <cmath>

int totalItems = 2;
int invX = 0;
int invY = 0;
std::regex command("[0-9]+|:", std::regex_constants::ECMAScript);


std::string cmdCur = "";
bool gameRun = true;
char userIn = '\0';
int playerX = 1500;
int playerY = 1500;
bool playerCollision = true;
bool commandMode = false;
const int scrnStrtPos = 1493;
int scrnX = scrnStrtPos;
int scrnY = scrnStrtPos;
std::string output = "";



struct Tile
{
    std::string disp;
    bool collide;
};

std::vector<Tile> test(10, { " ", false } );
std::vector<std::string> outRow(15, " ");
std::vector<Tile> infoRow(3000, { " ", false } );
std::vector<std::vector<std::string>> gameOut(15, outRow);
std::vector<std::vector<Tile>> tileInfo(3000, infoRow);

struct Inventory
{
    std::vector<std::vector<std::string>> Items;
    Inventory()
    {
        for(int i=0;i<15;i++)
        {
            Items.push_back( {".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."} );
        }
    }
};
struct InvData
{
    std::vector<std::vector<int>> Items;
    InvData()
    {
        for(int i=0;i<15;i++)
        {
            Items.push_back( {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} );
        }
    }
};



void house(std::vector<std::vector<Tile>> &vect, int x, int y)
{
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
        {
            if(vect[y+i][x+j].disp!=" ")
            {
                return;
            }
        }
    }
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
        {
            if(i==0||i==4&&j!=1)
            {
                vect[y+i][x+j].disp = "-";
                vect[y+i][x+j].collide = true;
            } else if(j==0||j==4)
            {
                vect[y+i][x+j].disp = "|";
                vect[y+i][x+j].collide = true;
            } else if(j==2&&i==2)
            {
                if(rand()%24==0)
                {
                    vect[y+i][x+j].disp = "1";
                } else if(rand()%24==0)
                {
                    vect[y+i][x+j].disp = "/";
                }
            }
        }
    }
}
void pii(std::string item, std::vector<std::vector<std::string>> &vect, std::vector<std::vector<int>> &dat, std::string heldItem)
{
    for(int i=0;i<vect.size();i++)
    {
        for(int j=0;j<vect.size();j++)
        {
            if(vect[i][j]==item||(invY==i&&invX==j&&heldItem==item))
            {
                dat[i][j]++;
                return;
            }
        }
    }
    for(int i=0;i<vect.size();i++)
    {
        for(int j=0;j<vect.size();j++)
        {
            if(vect[i][j]==".")
            {
                vect[i][j] = item;
                dat[i][j]++;
                totalItems++;
                return;
            }
        }
    }
}
int main()
{
    srand (time(NULL));
    Inventory plrInv;
    InvData PID;
    plrInv.Items[0][0] = "/";
    plrInv.Items[0][1] = "1";
    PID.Items[0][0] = 99;
    PID.Items[0][1] = 99;
    std::string heldItem = plrInv.Items[0][0];
    std::cout << "\33[38;5;46m";
    house(tileInfo, 150, 100);
    for(int i=0;i<100000;i++)
    {
        int ranX = rand()%2994 + 1;
        int ranY = rand()%2994 + 1;
        if(abs(ranX-playerX)<15&&abs(ranY-playerY)<15)
        {
            continue;
        }
        house(tileInfo, ranX, ranY);
    }
    for(int i=0;i<tileInfo.size();i++)
    {
        for(int j=0;j<tileInfo[i].size();j++)
        {
            if(i==0||i==2999)
            {
                tileInfo[i][j].disp = "-";
                tileInfo[i][j].collide = true;
            } else if(j==0||j==2999)
            {
                tileInfo[i][j].disp = "|";
                tileInfo[i][j].collide = true;
            }
        }
    }
    while(gameRun)
    {
        for(int i=0;i<gameOut.size();i++)
        {
            for(int j=0;j<gameOut[i].size();j++)
            {
                try
                {
                    gameOut[i][j] = tileInfo.at(scrnY+i).at(scrnX+j).disp;
                } catch (std::out_of_range)
                {
                    gameOut[i][j] = " ";
                }
                if(i==0||i==14)
                {
                    gameOut[i][j]= "-";
                } else if(j==0||j==14)
                {
                    gameOut[i][j] = "|";
                }
            }
        }
        gameOut[7][7] = "#";
        output = "";
        std::cout<< "\33[2J\33[1;1H";
        std::cout << std::flush;
        for(int i=0;i<gameOut.size();i++)
        {
            for(int j=0;j<gameOut[i].size();j++)
            {
                output += gameOut[i][j].substr(0, 1)+" ";
            }
            output += "\n";
        }
        plrInv.Items[invY][invX] = "#";
        output += "\33[1mInventory\33[22m\n";
        for(int i=0;i<plrInv.Items.size();i++)
        {
            for(int j=0;j<plrInv.Items[i].size();j++)
            {
                output += plrInv.Items[i][j].substr(0, 1)+" ";
            }
            output += "\n";
        }
        std::cout << output << "\33[1mPlayer X: " << playerX << "\nPlayer Y: " << playerY << "\ninvX: " << invX << "\ninvY: " << invY << "\nCurrent Item: " << heldItem << "\nCurrent Item Amount: " << PID.Items[invY][invX] << "\33[22m\n";
        std::cout << cmdCur;
        system("stty raw");
        userIn = getchar();
        system("stty cooked");
        if(cmdCur!="")
        {
            commandMode = true;
        } else
        {
            commandMode = false;
        }
        if(!commandMode)
        {
            switch (userIn)
            {
                case 'w':
                    try
                    {
                        if((!tileInfo.at(playerY-1).at(playerX).collide)||(playerY!=0&&!playerCollision))
                        {
                            scrnY--;
                            playerY--;
                        }
                    } catch(std::out_of_range)
                    {
                        scrnY--;
                        playerY--;
                    }
                    break;
                case 'W':
                    try
                    {
                        if((!tileInfo.at(playerY-1).at(playerX).collide)||(playerY!=0&&!playerCollision))
                        {
                            scrnY--;
                            playerY--;
                        }
                    } catch(std::out_of_range)
                    {
                        scrnY--;
                        playerY--;
                    }
                    break;
                case 'a':
                    try
                    {
                        if((!tileInfo.at(playerY).at(playerX-1).collide)||(playerX!=0&&!playerCollision))
                        {
                            scrnX--;
                            playerX--;
                        }
                    } catch(std::out_of_range)
                    {
                        scrnX--;
                        playerX--;
                    }
                    break;
                case 'A':
                    try
                    {
                        if((!tileInfo.at(playerY).at(playerX-1).collide)||(playerX!=0&&!playerCollision))
                        {
                            scrnX--;
                            playerX--;
                        }
                    } catch(std::out_of_range)
                    {
                        scrnX--;
                        playerX--;
                    }
                    break;
                case 's':
                    try
                    {
                        if((!tileInfo.at(playerY+1).at(playerX).collide)||(playerY!=14&&!playerCollision))
                        {
                            scrnY++;
                            playerY++;
                        }
                    } catch(std::out_of_range)
                    {
                        scrnY++;
                        playerY++;
                    }
                    break;
                case 'S':
                    try
                    {
                        if((!tileInfo.at(playerY+1).at(playerX).collide)||(playerY!=14&&!playerCollision))
                        {
                            scrnY++;
                            playerY++;
                        }
                    } catch(std::out_of_range)
                    {
                        scrnY++;
                        playerY++;
                    }
                    break;
                case 'd':
                    try
                    {
                        if((!tileInfo.at(playerY).at(playerX+1).collide)||(playerX!=14&&!playerCollision))
                        {
                            scrnX++;
                            playerX++;
                        }
                    } catch(std::out_of_range)
                    {
                        scrnX++;
                        playerX++;
                    }
                    break;
                case 'D':
                    try
                    {
                        if((!tileInfo.at(playerY).at(playerX+1).collide)||(playerX!=14&&!playerCollision))
                        {
                            scrnX++;
                            playerX++;
                        }
                    } catch(std::out_of_range)
                    {
                        scrnX++;
                        playerX++;
                    }
                    break;
                case 'i':
                    try
                    {
                        if(heldItem=="1")
                        {
                            tileInfo.at(playerY-1).at(playerX).disp = " ";
                            tileInfo.at(playerY-1).at(playerX).collide = false;
                        } else if(heldItem=="/"&&tileInfo.at(playerY-1).at(playerX).disp!=" "&&totalItems<225)
                        {
                            pii(tileInfo.at(playerY-1).at(playerX).disp, plrInv.Items, PID.Items, heldItem);
                            tileInfo.at(playerY-1).at(playerX).disp = " ";
                            tileInfo.at(playerY-1).at(playerX).collide = false;
                        }
                    } catch(std::out_of_range){}
                    break;
                case 'j':
                    try
                    {
                        if(heldItem=="1")
                        {
                            tileInfo.at(playerY).at(playerX-1).disp = " ";
                            tileInfo.at(playerY).at(playerX-1).collide = false;
                        } else if(heldItem=="/"&&tileInfo.at(playerY).at(playerX-1).disp!=" "&&totalItems<225)
                        {
                            pii(tileInfo.at(playerY).at(playerX-1).disp, plrInv.Items, PID.Items, heldItem);
                            tileInfo.at(playerY).at(playerX-1).disp = " ";
                            tileInfo.at(playerY).at(playerX-1).collide = false;
                        }
                    } catch(std::out_of_range){}
                    break;
                case 'k':
                    try
                    {
                        if(heldItem=="1")
                        {
                            tileInfo.at(playerY+1).at(playerX).disp = " ";
                            tileInfo.at(playerY+1).at(playerX).collide = false;
                        } else if(heldItem=="/"&&tileInfo.at(playerY+1).at(playerX).disp!=" "&&totalItems<225)
                        {
                            pii(tileInfo.at(playerY+1).at(playerX).disp, plrInv.Items, PID.Items, heldItem);
                            tileInfo.at(playerY+1).at(playerX).disp = " ";
                            tileInfo.at(playerY+1).at(playerX).collide = false;
                        }
                    } catch(std::out_of_range){}
                    break;
                case 'l':
                    try
                    {
                        if(heldItem=="1")
                        {
                            tileInfo.at(playerY).at(playerX+1).disp = " ";
                            tileInfo.at(playerY).at(playerX+1).collide = false;
                        } else if(heldItem=="/"&&tileInfo.at(playerY).at(playerX+1).disp!=" "&&totalItems<225)
                        {
                            pii(tileInfo.at(playerY).at(playerX+1).disp, plrInv.Items, PID.Items, heldItem);
                            tileInfo.at(playerY).at(playerX+1).disp = " ";
                            tileInfo.at(playerY).at(playerX+1).collide = false;
                        }
                    } catch(std::out_of_range){}
                    break;
                case 'I':
                    try
                    {
                        if(tileInfo.at(playerY-1).at(playerX).disp==" "&&heldItem!=".")
                        {
                            tileInfo.at(playerY-1).at(playerX).disp = heldItem;
                            if(heldItem=="|"||heldItem=="-")
                            {
                                tileInfo.at(playerY-1).at(playerX).collide = true;
                            }
                            PID.Items[invY][invX]--;
                            if(PID.Items[invY][invX]<1)
                            {
                                totalItems--;
                                heldItem = ".";
                            }
                        }
                    } catch(std::out_of_range){}
                    break;
                case 'J':
                    try
                    {
                        if(tileInfo.at(playerY).at(playerX-1).disp==" "&&heldItem!=".")
                        {
                            tileInfo.at(playerY).at(playerX-1).disp = heldItem;
                            if(heldItem=="|"||heldItem=="-")
                            {
                                tileInfo.at(playerY).at(playerX-1).collide = true;
                            }
                            PID.Items[invY][invX]--;
                            if(PID.Items[invY][invX]<1)
                            {
                                totalItems--;
                                heldItem = ".";
                            }
                        }
                    } catch(std::out_of_range){}
                    break;
                case 'K':
                    try
                    {
                        if(tileInfo.at(playerY+1).at(playerX).disp==" "&&heldItem!=".")
                        {
                            tileInfo.at(playerY+1).at(playerX).disp = heldItem;
                            if(heldItem=="|"||heldItem=="-")
                            {
                                tileInfo.at(playerY+1).at(playerX).collide = true;
                            }
                            PID.Items[invY][invX]--;
                            if(PID.Items[invY][invX]<1)
                            {
                                totalItems--;
                                heldItem = ".";
                            }
                        }
                    } catch(std::out_of_range){}
                    break;
                case 'L':
                    try
                    {
                        if(tileInfo.at(playerY).at(playerX+1).disp==" "&&heldItem!=".")
                        {
                            tileInfo.at(playerY).at(playerX+1).disp = heldItem;
                            if(heldItem=="|"||heldItem=="-")
                            {
                                tileInfo.at(playerY).at(playerX+1).collide = true;
                            }
                            PID.Items[invY][invX]--;
                            if(PID.Items[invY][invX]<1)
                            {
                                totalItems--;
                                heldItem = ".";
                            }
                        }
                    } catch(std::out_of_range){}
                    break;
                case 'x':
                    PID.Items[invY][invX]--;
                    if(PID.Items[invY][invX]<1)
                    {
                        totalItems--;
                        heldItem = ".";
                    }
                    break;
                case 'X':
                    PID.Items[invY][invX] = 0;
                    totalItems--;
                    heldItem = ".";
                    break;
                case '\177':
                    playerCollision = !playerCollision;
            }
        }
        std::string inChar(1, userIn);
    
        if(std::regex_search(inChar, command)||(cmdCur.substr(0, 1)==":"&&inChar!=" "&&inChar!="\r"))
        {
            if(inChar=="\177"&&(cmdCur.size()!=1&&cmdCur.substr(0, 1)==":"))
            {
                cmdCur = cmdCur.substr(0, cmdCur.size()-1);
            } else
            {
                cmdCur += userIn;
            }

        } else if(userIn==' ')
        {
            try
            {
                try
                {
                    if(cmdCur!="")
                    {
                        if(stoi(cmdCur)>225||stoi(cmdCur)<1)
                        {
                            cmdCur = "";
                        }
                    }
                    plrInv.Items[invY][invX] = heldItem;
                    invY = std::floor((stoi(cmdCur)-1)/15);
                    invX = stoi(cmdCur)-(invY*15)-1;
                    heldItem = plrInv.Items[invY][invX];
                    plrInv.Items[invY][invX] = "#";
                    cmdCur = "";
                } catch(std::out_of_range)
                {
                    cmdCur = "";
                }
            } catch(std::invalid_argument)
            {
                if(cmdCur.substr(0, 5)==":give")
                {
                    if(cmdCur.size()!=5)
                    {
                        pii(cmdCur.substr(5, cmdCur.size()-5), plrInv.Items, PID.Items, heldItem);
                    }
                    cmdCur = "";
                }
                if(cmdCur.substr(0, 9)==":multgive")
                {
                    if(cmdCur.size()>12)
                    {
                        for(int i=0;i<stoi(cmdCur.substr(9, 3));i++)
                        {
                            pii(cmdCur.substr(12, cmdCur.size()-12), plrInv.Items, PID.Items, heldItem);
                        }
                    }
                    cmdCur = "";
                }
            }
        }
        
    }
    return 0;
}