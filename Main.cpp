#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
using namespace std;

// Base class
class Base {
protected:
  int health;
  int meleeDamange;
  int lastDamageTaken; // Amount of damage onlast attack

public:
  virtual ~Base() = default;
  void setHealth(int _health) {
    health = _health;
    lastDamageTaken = 0;
  }

  int getHealth() { return health; }
  /* Take Damage Function */
  virtual void TakeDamage(int _damage) {
    health -= _damage;
    lastDamageTaken = _damage;
  }

  virtual int attackMelee() = 0;
  virtual int Heal() = 0;
};

/*Enemy Base Class*/
class Enemy : public Base {
public:
  Enemy() = default;
  Enemy(int _currentLevel) {
    health = 50 + (5 * _currentLevel) / 2;
    meleeDamange = 5 + (5 * _currentLevel) / 2;
  }

  virtual ~Enemy(){};
  virtual char RandomChoice() // Random Choice
  {
    return (rand() % 5) ? 'M' : 'H'; // H(20%) M (80%)
  }
  /*Enemy attack*/
  virtual int attackMelee() override {
    cout << "The enemy fights back with " << meleeDamange << " damage.\n";
    return meleeDamange;
  }

  /*Enemy Heal*/
  virtual int Heal() override {
    cout << "Behold! The enemy got healed\n";
    health += 5;
    return 0;
  }

  /*Enemy Take Damage*/
  virtual void TakeDamage(int _damage) override {
    if (!(bool)(rand() % 10)) // 10% Chance
    {
      cout << "The Enemy has blocked your damage!!\n";
      _damage = 0;
    }
    lastDamageTaken = _damage;
    health -= _damage;
  }
};

/*Bass Enemy*/
class BossEnemy : public Enemy // Murlocks
{
public:
  BossEnemy() {
    health = 100;
    meleeDamange = 20;
  }
  char RandomChoice() override { return (rand() % 5) ? 'M' : 'H'; }

  int attackMelee() override {
    cout << "Murlocks hits you with his weapon, inflicting " << meleeDamange
         << " damage.\n";
    return meleeDamange;
  }

  virtual int Heal() override {
    cout << "Behold! The Murlocks got healed\n";
    health += 10;
    return 0;
  }

  void TakeDamage(int _damage) override {
    if (!(bool)(rand() % 5)) {
      cout << "Murlock has blocked your damage with his shield\n";
      _damage = 0;
    }
    lastDamageTaken = _damage;
    health -= _damage;
  }
};

/*Player Class*/
class Player : public Base {

  int rangedDamage;
  int currentLevel;
  int specialAbilityChance;
  bool isInvincible;
  map<string, int> specialAbilities;
  map<string, bool> gearsOfArms;

public:
  Player() {
    health = 500;
    meleeDamange = 10;
    rangedDamage = 5;
    isInvincible = false;
    currentLevel = 1; // Starting level
    specialAbilityChance = 10;
    specialAbilities = {{"CriticalHit", false},
                        {"Blocker", false},
                        {"LifeSteal", false},
                        {"RangedAttack", false}};
    gearsOfArms = {{"Map", false},
                   {"Sword", false},
                   {"Shield", false},
                   {"Armour", false},
                   {"Bow", false}};
  }

  void DisplayPlayerStats() {
    cout << "PLAYER STATS:\n";
    cout << "Health of Player: " << health << "\n";
    cout << "Melee Damage: " << meleeDamange
         << "\t Ranged Damage: " << rangedDamage << "\n";
    cout << "Gears of Arms Collected: ";
    for (const auto &ele : gearsOfArms) {
      if (ele.second == true)
        cout << ele.first << ", ";
    }
    cout << endl;
    cout << "Special Abilities Unlocked: ";
    for (const auto &ele : specialAbilities) {
      if (ele.second == true)
        cout << ele.first << ", ";
    }
    cout << endl;
  }
  /*update items found*/
  void setGearsofArms(string _gear) {
    cout << "Player has discovered a new Gear of Arm: " << _gear << endl;
    gearsOfArms[_gear] = true;
  }
  /*update special ability unlocked*/
  void setSA(string _ability) {
    cout << "Player has unlocked a new Special Abiliity: " << _ability << endl;
    specialAbilities[_ability] = true;
  }

  void setCurrentLevel(int _level) { currentLevel = _level; }
  int getCurrentLevel() { return currentLevel; }

  // method to return how much damage to do
  int attackMelee() override {
    int damageToDo = meleeDamange; // base damage
    if (specialAbilities["CriticalHit"] == true && !(bool)(rand() % 5)) {
      damageToDo += meleeDamange * 5;
      cout << "\nCRITICAL HIT!!!!!!!!\n";
    } else if (specialAbilities["LifeSteal"] == true && !(bool)(rand() % 5)) {
      cout << "!!!Special Ability Life-Steal Activated!!!! Player gains some "
              "health.";
      health += 3 * currentLevel;
    }
    cout << "Your Player slams his weapon across the enemy with " << damageToDo
         << " damage.\n";
    return damageToDo;
  }
  int attackRanged() {

    if (specialAbilities["RangedAttack"] == true && !(bool)(rand() % 5)) {
      cout << "!!! Ranged Hit Special Ability Activated !!!\n";
      cout << "Your Player is invinsible for the next turn!\n";
      isInvincible = true;
    }
    cout << "Your Player sneaks a ranged atack towards the enemy with "
         << rangedDamage << "!\n";
    return rangedDamage;
  }
  int Heal() override // Player heals himself
  {
    cout << "Player got healed himself!\n";
    health += (currentLevel * 20);
    return 0;
  }
  virtual void TakeDamage(int _damage) override

  {
    if (specialAbilities["Blocker"] == true && !(bool)(rand() % 5)) {
      cout << "!!!Special Ability Blocked Activated!!!! Player takes no "
              "damage.\n";
      _damage = 0;
    }
    if (isInvincible) {
      cout << "You are in invincible mode, Player takes no damage >:)\n";
      _damage = 0;
    }
    lastDamageTaken = _damage;
    health -= _damage;
  }

  void setSpecialAbilityChance(int _chance) { specialAbilityChance = _chance; }
  ~Player() { cout << "Your Player died\n"; }
};
class Level {
private:
  int numOfEnemies;
  int levelNumber;
  vector<std::shared_ptr<Enemy>> enemies;
  const char *const specialAbilities[4] = {"CriticalHit", "Blocker",
                                           "LifeSteal", "RangedAttack"};
  const char *const gearsOfArms[5] = {"Map", "Sword", "Shield", "Armour",
                                      "Bow"};
  void generateEnemies() {
    for (int i = 0; i < levelNumber; i++) {
      enemies.push_back(std::shared_ptr<Enemy>(new Enemy(levelNumber)));
    }
  }

public:
  Level(int _level) {
    numOfEnemies = _level;
    levelNumber = _level;
    generateEnemies();
  }

  int getLevelNumber() { return levelNumber; }
  int getNumOfEnemies() { return numOfEnemies; }
  string getGearOfArms(int _level) { return gearsOfArms[_level - 1]; }
  string getSpecialAbility(int _level) { return specialAbilities[_level - 1]; }

  vector<std::shared_ptr<Enemy>> listOfEnemies() { return enemies; }
};

class Game {
  unique_ptr<Player> player;

public:
  void Start() {
    Monologue();
    SpawnPlayer();
  }

public:
  void Update() {
    if (!player) {
      cout << "Player  has not been spawned correctly!\n";
      return;
    }
    for (int i = 1; i < 6; i++) {
      player->DisplayPlayerStats();
      LoadLevel(i);
    }
    player->DisplayPlayerStats();
    FinalLevel();
  }

private:
  void Monologue() // back story
  {
    cout
        << "\nAt the dawn of the universe, there was nothing The Big Bang sent "
           "six elemental crystals hurtling across the virgin universe. These "
           "Infinity Stones each control an essential aspect of existence. "
           "there were 6 infinity stones namely Space, Reality, Power, Soul, "
           "Mind, and Time. Which were hidden in resemblance object like "
           "Map(Space Stone), Sword(Power Stone), Shield (Soul Stone), Armour "
           "(Mind Stone) & Bow(Time Stone) which were guarded by Uatus\n";

    cout << "\nLord of the Skrulls Murlock half blood demon based on Andromeda "
            "had obtained Reality Stone during  2nd  Kree -Skrulls war and got "
            "to know about other infinity stones. So he is on the way to hand "
            "on all the infinity stones\n";

    cout << "\nOur Player, torch bearer of his clan,  begins his journey in "
            "quest of all the gear arms.\nHe must find them all to rescue his "
            "clan.\n";
  }

  
  void SpawnPlayer() // instantiate the player
  {
    player = make_unique<Player>();
  }

  void LoadLevel(int _currentPlayerLevel) // basic mechanism of first 5 levels
  {

    unique_ptr<Level> currentLevel = make_unique<Level>(_currentPlayerLevel);
    cout << "You have stepped in to level " << _currentPlayerLevel << endl;
    vector<std::shared_ptr<Enemy>> levelEnemies = currentLevel->listOfEnemies();
    cout << "This level has " << currentLevel->getNumOfEnemies()
         << " enemies\n";
    std::shared_ptr<Enemy> e1 = levelEnemies[0];
    while (player->getHealth() > 0 &&
           e1->getHealth() > 0) // while the player is alive and enemy is alive
    {
      cout << "Quick Press H (Heal) or M (Melee Attack) or R (Ranged Attack)\n";
      char ch;
      do {
        cin >> ch;
      } while (ch != 'H' && ch != 'M' && ch != 'R'); // player enters his action
      int damageToEnemy = PlayerChoice(ch);
      e1->TakeDamage(damageToEnemy); // damage to enemy
      int damageToPlayer =
          EnemyChoice(e1->RandomChoice(), e1); // random enemy action
      player->TakeDamage(damageToPlayer);      // damage to player
      cout << "Player Health is: " << player->getHealth() << endl;
      cout << "Enemy Health is: " << e1->getHealth() << endl;
      if (e1->getHealth() < 1) // is one enemy is dead and there are more left
      {
        cout << "The enemy has been killed!\n";
        levelEnemies.erase(levelEnemies.begin());
        if (!levelEnemies.empty()) {

          e1 = levelEnemies[0];
          cout << "Another enemy appears with " << e1->getHealth() << "\n";
        }
      }
    }
    if (player->getHealth() < 1) // if player is dead
    {
      cout << "Your Player died in the war.\n The Uatus are "
              "victorious\n";
    }
    player->setGearsofArms(currentLevel->getGearOfArms(_currentPlayerLevel));
    player->setSA(currentLevel->getSpecialAbility(_currentPlayerLevel));
  }

  int PlayerChoice(char _ch) // player action decider based on the input
  {
    if (_ch == 'H') {
      player->Heal();
      return 0;
    } else if (_ch == 'M') {
      return player->attackMelee();
    } else {
      return player->attackRanged();
    }
  }

  int EnemyChoice(char _ch,
                  shared_ptr<Enemy> Base) // enemy action decider based on
                                          // random generated input
  {
    if (_ch == 'M') {
      return Base->attackMelee();
    } else {
      return Base->Heal();
    }
  }

private:
  void FinalLevel() // logic for boss level
  {

    cout << "You have stepped in to level " << 6 << endl;
    cout << "THIS IS THE FINAL LEVEL!!! MURLOCS ARRIVES!!\n";
    shared_ptr<BossEnemy> e1 = make_shared<BossEnemy>(); // spawn boss
    player->setSpecialAbilityChance(
        5); // increase player chance for special ability, 5 = 20%
    while (player->getHealth() > 0 && e1->getHealth() > 0) {
      cout << "Quick Press H (Heal) or M (Melee Attack) or R (Ranged Attack)\n";
      char ch;
      do {
        cin >> ch;
      } while (ch != 'H' && ch != 'M' && ch != 'R');
      int damageToEnemy = PlayerChoice(ch);
      e1->TakeDamage(damageToEnemy);
      int damageToPlayer = EnemyChoice(e1->RandomChoice(), e1);
      player->TakeDamage(damageToPlayer);
      cout << "Player Health is: " << player->getHealth() << endl;
      cout << "Enemy Health is: " << e1->getHealth() << endl;
    }
    if (player->getHealth() < 1) {
      cout << "Your Player dies of wounds of the war.\n The Murlocs is "
              "victorious.\n";
    }
  }
};

int main() {

  srand(time(0)); // random int
  Game game;
  game.Start();
  game.Update();
  return 0;
}
