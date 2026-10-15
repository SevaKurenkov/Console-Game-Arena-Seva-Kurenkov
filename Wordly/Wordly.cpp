// Wordly.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

class Fighter {
private:
    const char* name;
    int health;
    int maxHealth;
    int damage;
public:
    Fighter(const char* nameF, int healthF, int damageF) {
        name = nameF;
        health = healthF;
        damage = damageF;
        maxHealth = healthF;
    }
    void TakeDamage(int amount) {
        if (amount >= health) {
            health = 0;
        }
        else {
            health = health - amount;
        }
    }
    void Attack(Fighter& target) const {
        target.TakeDamage(damage);
    }
    bool IsAlive() const {
        if (health > 0) {
            return true;
        }
        else {
            return false;
        }
    }
    void PrintStatus() const {
        std::cout << "Имя: " << name << "\n" << "Здоровье: " << health << "\n" << "Максимальное здоровье: " << maxHealth << "\n" << "Урон: " << damage << "\n" << "\n";
    }
    void Heal(int amount) {
        if (health + amount >= maxHealth) {
            health = maxHealth;
        }
        else
        {
            health += amount;
        }
    }
};

class Game {
private:
    Fighter player;
    Fighter enemies[3];
    Fighter* currentTarget = nullptr;
    int medkits = 1;
public:

    Game()
        : player("Hero", 20, 8),
        enemies{ {"Enemy 1", 12, 3},
        {"Enemy 2", 18, 4},
        {"Enemy 3", 24, 5} } {
    }

    void Run() {
        PrintFighters(player, enemies, currentTarget);
        int comand = 1;
        while (true){
            std::cout << "Доступные команды:" << "\n" << "0 - выйти" << "\n" << "1 - выбрать цель (затем ввести номер 1-3)" << "\n" << "2 - атаковать выбранную цель" << "\n" << "3 - использовать аптечку" << "\n" << "\n";
            std::cin >> comand;
            std::cout << "\n";
            if (comand == 0) {
                std::cout << "Выбрана команда выхода" << "\n" << "\n";
                break;
            }
            if (comand == 1) {
                std::cout << "Выбрана команда выбора цели" << "\n" << "\n";
                int target = -1;
                std::cin >> target;
                while (!TrySelectTarget(target, currentTarget)){
                    std::cout << "Ошибка: Цель уже мёртва или неверная, введите цель заново" << "\n" << "\n";
                    std::cin >> target;
                }
                std::cout << "Цель изменена" << "\n" << "\n";
            }
            if (comand == 2) {
                std::cout << "Выбрана команда атаки выбранной цели" << "\n" << "\n";
                if (currentTarget == nullptr) {
                    std::cout << "Ошибка: Цель уже мёртва или не выбрана, введите другую цель" << "\n" << "\n";
                }
                if (currentTarget != nullptr) {
                    player.Attack(*currentTarget);
                    if (AllEnemiesDefeated()) {
                        PrintFighters(player, enemies, currentTarget);
                        std::cout << "Игрок победил!" << "\n" << "\n";
                        break;
                    }
                    if (currentTarget->IsAlive()) {
                        currentTarget->Attack(player);
                        PrintFighters(player, enemies, currentTarget);
                        if (!player.IsAlive()) {
                            std::cout << "Игрок проиграл!" << "\n" << "\n";
                            break;
                        }
                    }
                    else {
                        currentTarget = nullptr;
                        PrintFighters(player, enemies, currentTarget);
                    }
                }
            }
            if (comand == 3) {
                std::cout << "Выбрана команда использования аптечки" << "\n" << "\n";
                if (player.IsAlive() && medkits > 0) {
                    player.Heal(8);
                    medkits--;
                    if (currentTarget->IsAlive()) {
                        currentTarget->Attack(player);
                        PrintFighters(player, enemies, currentTarget);
                        if (!player.IsAlive()) {
                            std::cout << "Игрок проиграл!" << "\n" << "\n";
                            break;
                        }
                    }
                }
                else {
                    std::cout << "Ошибка: Нет оптечек или игрок мёртв!" << "\n" << "\n";
                }
            }
        }

    }

    void PrintFighter(const Fighter& fighter) {
        fighter.PrintStatus();
    }

    void PrintFighters(const Fighter& hero, const Fighter* enemiesPrint, Fighter*& target) {
        PrintFighter(hero);
        for (int i = 0; i < 3; ++i) {
            PrintFighter(enemiesPrint[i]);
        }
        std::cout << "Текущая цель: ";
        if (target != nullptr) {
            target->PrintStatus();
        }
        else {
            std::cout << "нет цели" << "\n" << "\n";
        }
        std::cout << "Аптечки: " << medkits << "\n" << "\n";
        
    }

    bool TrySelectTarget(int number, Fighter*& outTarget) { 
        outTarget = nullptr;
        if ((1 <= number) && ((3 >= number)) && enemies[number - 1].IsAlive()) {
            outTarget = &enemies[number - 1];
            return true;
        }
        else {
            return false;
        }
    }

    bool AllEnemiesDefeated() const {
        for (int i = 0; i < 3; ++i) {
            if (enemies[i].IsAlive()) {
                return false;
            }
        }
        return true;
    }

};

int main()
{
    Game game = Game();
    game.Run();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
