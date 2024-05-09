#include "game.h"
#include <iostream>
#include <vector>
#include <thread>

class Print {
public:
    void print() { std::cout << "Hello there!\n"; }
};


// Simulated class for multiplayer interaction. Basic HP and Damage system.
class Base {
public:
    int HP = 100;
    int baseDamage = 5;

    std::string Name;

    Base* Opponent = nullptr;


    int takeDamage() {
        return HP - Opponent->baseDamage;
    }

    void run() {
        while (HP > 0) {
            int damage = HP - takeDamage();
            HP = HP - damage;
            std::cout << Name + " has taken " + std::to_string(damage) + " damage from " + Opponent->Name + "\n";
        }

        std::cout << Name + " has died first! \n";
        return;
    }

};

// Basic Loop class, used to see how much a thread can output before a secondary thread determines it to stop.
class Loop {
public:

    bool hasToStop;

    void run() {
        while (hasToStop != true) {
            std::cout << "Still in the loop\n";
        }
    }
};

int main()
{
    std::cout << "Hello World!";

    srand(time(NULL));

    // Print p;
    // Loop l;

    // Base rol;
    // Base dan;
    
    // Basic Thread development with Lamdba value.
    // std::thread t([&p]() {
    //     p.print();
    // });
    // t.join();

    // We will create a Time Loop Thread
    // std::thread tL(&Loop::run, &l);

    // // We create a Time Management thread which determines the "hasToStop" boolean to true. Then rejoins.
    // std::thread tTM([&l]() {
    //     // Uses std::chrono to determine a random time.
    //     std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));
    //     l.hasToStop = true;
    // });
    // tTM.join();

    // // Rejoin the Time Loop Thread.
    // tL.join();


    // // Basic multi-thread development
    // //std::vector<std::thread> threads;

    // //for (int i = 0; i < 6; i++) {
    // //    threads.push_back(std::thread(&Print::print, &p));
    // //}
    // //for (auto& thread : threads) {
    // //    thread.join();
    // //}

    // rol.Name = "Rolfor";
    // dan.Name = "Dan the Guard";

    // rol.Opponent = &dan;
    // dan.Opponent = &rol;

    // std::thread RolforThread(&Base::run, &rol);
    // std::thread DanThread(&Base::run, &dan);

    // RolforThread.join();
    // DanThread.join();

    game g;
    g.run();

    return 0;
}