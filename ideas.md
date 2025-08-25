# Brainstorm

Risk it for the biscuit
- Gambling
- It's very difficult to get to the goal
- The more an action is done the probability of success goes down, but at the same time the reward is higher

The more an action is done the probability of success goes down, but at the same time the reward is higher
- You can waste money to buy something of high value, gambling basically


Waves of enemies come up, you receive for many with each wave. The wave becomes, obviously, more difficult with time. It is possible to give up th run, this you just give you the amount of money you got or you can keep going. If you die you lose all the money from the run and the money you have, basically it becomes sort of a cookie cliker with extras steps while having a rogue-like thing on top and a bit of gambling because everyone loves gambling.

What do you buy with the money?
- Guns? like a hack n slash thing?
- Abilities? maybe this is too complicated?

I want "infinite" replayability, this means I would need a lot of different abilities and a lot of options with the money usage. Maybe that's fine as jam game because I can fake this. Everyone will play it for 5 minutes maximum so \~10 things that you're able to able is enough, maybe at the end of the run this things are randomized by sets of 3 based on the run performance?

Maybe instead of money you receive some sort of "points" at the end of the run based on the points you have you'll be able to select between three items or abilities.

# Gameplay loop
- You start the run at the shop with 100 gold
- A battle arena will be generated procedurally at the start of the level
- A count down will begin (or something like that)
- Waves of enemies will start to appear
- You try to survive and kill the enemies in the arena using your items and abilities
- The enemies will come in waves, each wave will bring stronger and more enemies 
- Through out the battle you will be receiving gold
    - You receive a certain amount of gold depending on the type of enemy you kill
    - You gain gold by wave completion, the amount of gold is exponentioal (10 gold on first wave, 20 on second, 40 on third, 60 on the fourth, etc)
- After two minutes in battle have passed it will become possible to stop the battle
- When a battle is stopped you enter the "shop mode", it'll look something like the shop mode in super auto pets
- If you're killed all your items, abilities and accumulated gold vanishes. Basically you lose the run.

The game will be basically an sort of RPG thingy.

# Story
Since the beginning of the new era the people of the kingdom have being starving, suffering and slowly dying. The only hope for stopping a revolution to occur is your the entreteniment caused from your suffering. Battle for the people.

# Attributes
Attributes are certain characteristics of your character.

At the start of each run your attributes are all 0.

On the shop you can level up your attributes with gold. The maximum an attribute can go is 5.

Attribute price per attribute level:
| Attrib. | Price |
|---------|-------|
| 0       | $20   |
| 1       | $40   |
| 2       | $80   |
| 3       | $120  |
| 4       | $240  |
| 5       | $480  |

## Popularity
Your popularity between the people.

You gain a certain amount of gold every `N` amount of sconds. The higher your popularity `N` becomes smaller:
| Pop. | N          |
|------|------------|
| 0    | 30 seconds |
| 1    | 15 seconds |
| 2    | 8  seconds |
| 3    | 4  seconds |
| 4    | 2  seconds |
| 5    | 1  second  |

The amount of gold gained with your popularity is 1 by default. Abilities can make these value go up.

## Strength
Your physical strength.

Determines how effective your attacks are. The higher your strength your probabilities of a better attack also increases:
| Str. | Bad  | Mid | Good | Critic |
|------|------|-----|------|--------|
| 0    | 100% | 0%  | 0%   | 0%     |
| 1    | 60%  | 35% | 4%   | 1%     |
| 2    | 40%  | 50% | 9%   | 1%     |
| 3    | 20%  | 50% | 25%  | 5%     |
| 4    | 5%   | 40% | 40%  | 15%    |
| 5    | 0%   | 20% | 50%  | 30%    |

## Agility
Your coordination and speed.

Determines how effective fast your attacks are. The higher your agility the heaviness of weapons becomes less effective:
| Agi. | Heaviness impact   |
|------|--------------------|
| 0    | 100%               |
| 1    | 90%                |
| 2    | 70%                |
| 3    | 60%                |
| 4    | 50%                |
| 5    | 40%                |

## Health
Your physical health.

Determine how much HP you'll have. The higher your health your HP also increases:
| Hea. | HP   |
|------|------|
| 0    | 10   |
| 1    | 20   |
| 2    | 40   |
| 3    | 60   |
| 4    | 80   |
| 5    | 100  |

# Combat
The combat is real time combat.

Each attack you hit a roll will occur and a random _attack tier_ will be selected based on your [Strength](#Strength).

The attack tiers are:
1. Bad: You deal little damage
2. Mid: You deal avarage damage
3. Good: You deal a lot of damage
4. Critic: You deal the maximum amount of Good damage plus an attack damage (with its own hit tier roll and damage roll)

The damage per type of attack depends of your weapon.

When a damage is received you have 0.1 seconds of invicibility.

# Weapons
Weapons have a minimum amount of [Strength](#Strength) required.

They also have a heaviness value. This determines how much time of wait is needed in between attacks:
| Heaviness | Wait        |
|-----------|-------------|
| 0         | 0.1 seconds |
| 1         | 0.2 seconds |
| 2         | 0.3 seconds |
| 3         | 0.5 seconds |
| 4         | 0.8 seconds |
| 5         | 1 second    |

All weapons are always sold at the shop.

## Stick
__Strength:__ 0

__Price:__ $5

__Heaviness:__ 0

Damage per attack tier:
- __Bad:__ 1
- __Mid:__ 1
- __Good:__ 1\~2

## Wooden sword
__Strength:__ 1

__Heaviness:__ 1

__Price:__ $20

Damage per attack tier:
- __Bad:__ 1
- __Mid:__ 2
- __Good:__ 2\~3

## Blunt Sword
__Strength:__ 1

__Heaviness:__ 2

__Price:__ $40

Damage per attack tier:
- __Bad:__ 2
- __Mid:__ 3
- __Good:__ 4\~5

## Knife
__Strength:__ 1

__Heaviness:__ 1

__Price:__ $60

Damage per attack tier:
- __Bad:__ 2
- __Mid:__ 2\~5
- __Good:__ 3\~5

## Basic Sword
__Strength:__ 2

__Heaviness:__ 2

__Price:__ $100

Damage per attack tier:
- __Bad:__ 3\~4
- __Mid:__ 5\~10
- __Good:__ 8\~12

## Large sword
__Strength:__ 3

__Heaviness:__ 3

__Price:__ $225

Damage per attack tier:
- __Bad:__ 8\~20
- __Mid:__ 15\~25
- __Good:__ 30

## Battle axe
__Strength:__ 3

__Heaviness:__ 4

__Price:__ $300

Damage per attack tier:
- __Bad:__ 15\~20
- __Mid:__ 30\~40
- __Good:__ 35\~45

## War hammer
__Strength:__ 3

__Heaviness:__ 4

__Price:__ $300

Damage per attack tier:
- __Bad:__ 5\~25
- __Mid:__ 10\~50
- __Good:__ 15\~55

## Legendary sword 
__Strength:__ 5

__Heaviness:__ 5

__Price:__ $500

Damage per attack tier:
- __Bad:__ 20\~40
- __Mid:__ 30\~60
- __Good:__ 40\~80

# Cards
Cards are consumable items that are sold at the shop.

When a card is bought its effects will become active for the entirity of the next battle stage only. It is possible to by up to four cards before going into battle.

At the start of the run there is only one card slot in the shop, the card in it is randomized. It is possible to reroll the card by the price of $10. The first time clearing the second, fifth and tenth waves a new slot is unlocked (so there are 4 maximum slots). The slots reset after death.

It is possible to freeze specific slots of cards, the card will stay at that slot if rerolls are used and even in the next shop (if you don't die). If all slots are freezed reroll is not possible.

Each card has a _minimum amount of waves_ value, this determines how many waves you should pass in this run for that card to start appear at the shop.

Same effect do not stack, thus it is not possible to by it twice. In fact when an effect is bought (or freeze) it'll not appear again on the current shop. Cards also do not repeat by slots.

They also have a _rarity_ tier (Common, Rare, Legendary), this tier determines their chance at appearing at the shop:
| Rarity    | Chance |
|-----------|--------|
| Common    | 60%    |
| Rare      | 30%    |
| Legendary | 10%    |

## List of cards
- __Popular__
    - __Description:__ You're considered a promesing gladiator
    - __Effect:__ Instead of receiving just 1 gold with your popularity you receive 2
    - __Price:__ $20
    - __Rarity:__ Common
    - __Waves:__ 1
- __One with the people__
    - __Description:__ You cativated the people through your battles
    - __Effect:__ Instead of receiving just 1 gold with your popularity you receive 2\~4 (+1 if already has __Popular__)
    - __Price:__ $50
    - __Rarity:__ Rare
    - __Waves:__ 5
- __Iron body__
    - __Description:__ With some endurance training your body became partially resistent
    - __Effect:__ You receive only 90% of damage towards you
    - __Price:__ $20
    - __Rarity:__ Common
    - __Waves:__ 0 
- __Skin of armor__
    - __Description:__ With good endurance training your body became very resistent
    - __Effect:__ You receive only 75% of damage towards you (65% if already has __Iron body__)
    - __Price:__ $50
    - __Rarity:__ Rare
    - __Waves:__ 5 
- __Protected by the gods__
    - __Description:__ With painfully endurance training your body became extremely resistent
    - __Effect:__ You receive only 50% of damage towards you (45% if already has __Iron body__ or __Skin of armor__ and 40% if have both)
    - __Price:__ $100
    - __Rarity:__ Legendary
    - __Waves:__ 10
- __Overcoming__
    - __Description:__ Your body is resilient to pain
    - __Effect:__ Every minute you recover 1 HP
    - __Price:__ $50
    - __Rarity:__ Common
    - __Waves:__ 0 
- __The Resilient one__
    - __Description:__ Your body is extremely resilient to pain
    - __Effect:__ Every 30 seconds you recover 1 HP
    - __Price:__ $150
    - __Rarity:__ Legendary
    - __Waves:__ 10
- __Gold digger__
    - __Description:__ The only thing on your mind is money
    - __Effect:__ There is a 50% of chance of enemies dropping double the money
    - __Price:__ $40
    - __Rarity:__ Rare
    - __Waves:__ 1
- __Berserk__
    - __Description:__ The hate for your enemies is what fortifies you
    - __Effect:__ When your life is less than 20% your attack speed increases by 10% (it stacks with your [Agility](#Agility)) increases by 10% for 5s (the time does not stack)
    - __Price:__ $100
    - __Rarity:__ Rare
    - __Waves:__ 5
- __Invincible__
    - __Description:__ You're...
    - __Effect:__ When your life is less than 10% you have 1 second of invincibility instead of 0.1
    - __Price:__ $50
    - __Rarity:__ Rare
    - __Waves:__ 0
- __The Healthy One__
    - __Description:__ You're very healthy
    - __Effect:__ You have 5 extra HP
    - __Price:__ $20
    - __Rarity:__ Common
    - __Waves:__ 0
- __Fitness above all__
    - __Description:__ You're extremely healthy
    - __Effect:__ You have 10 extra HP
    - __Price:__ $30
    - __Rarity:__ Rare
    - __Waves:__ 5
- __Holy body__
    - __Description:__ You're the healthiest there is
    - __Effect:__ You have 20 extra HP
    - __Price:__ $60
    - __Rarity:__ Legendary
    - __Waves:__ 10
- __Trained fighter__
    - __Description:__ You've trained to be a proper fighter
    - __Effect:__ When you do Bad damage it'll have 75% of becoming a Mid damage and 25% of becoming a Good damage
    - __Price:__ $80
    - __Rarity:__ Rare
    - __Waves:__ 5
- __Perfect fighter__
    - __Description:__ You've trained to be a proper fighter
    - __Effect:__ When you do Bad damage it'll have 50% of becoming a Mid damage and 50% of becoming a Good damage (if already has __Trained fighter__ it becomes 25% to become Mid and 75% to become Good)
    - __Price:__ $160
    - __Rarity:__ Legendary
    - __Waves:__ 10


# Enemies
 
All enemies have the same [attributes](#Attributes) as the player (except [Popularity](#Popularity)) and their attack attributes will be similar to a [weapon](#Weapons) damage.

## Slimes
Slimes have a pretty dumb AI, slow attack and week damage.

__Heaviness of attack:__ 2

Damage per attack tier:
- __Bad:__ 1
- __Mid:__ 1\~2
- __Good:__ 3

Attributes:
- __Strength:__ 0\~2
- __Agility:__ 0\~2
- __Health:__ 0

Their HP will also have a small debuff, their max HP will be subtracted by 0\~6.

They just follow the player without any pathfinding and stop following when the player hides or goes too far.

## Skeletons
Skeletons have a smarter AI than slimes, their attacks are mostly fast and they do weak damage.

__Heaviness of attack:__ 1

Damage per attack tier:
- __Bad:__ 1
- __Mid:__ 1\~2
- __Good:__ 3

Attributes:
- __Strength:__ 0\~2
- __Agility:__ 0\~3
- __Health:__ 0\~2

Their HP will also have a small debuff, their max HP will be subtracted by 5\~15 (with a minimum of 4).

They have ranged weapons. They will only follow the player until a safe spot and start to shot at him. They will try to run away if the player follows them.

## Zombies
Zombies have a pretty smart AI, their attacks can be as strong and fast as the players.

They will have a random weapon.

Attributes:
- __Strength:__ 0\~5
- __Agility:__ 0\~5
- __Health:__ 0\~5

Their HP will also have a small debuff, their max HP will be subtracted by 0\~20 (with a minimum of 8).

Zombies also can have [card](#Cards) effects. They can have zero to two card effects at once.

They will pathfind the player always at a somewhat safe distance and attack from there.

# Wave progression
The first wave only has [slimes](#Slimes). It has 10\~20 slimes in it.

The second wave also is only slimes. It has from 20\~30 slimes in it.

The third wave only has [skeletons](#Skeletons). It has 8\~12 skeletons in it.

The fourth wave has skeletons and slimes in it. It has 5\~10 skeletons and 15\~20 slimes.

The fifth wave only has [zombies](#Zombies), but only no-card and basic weaponed ones. It has 10\~15 zombies in it.

The sixth wave has zombies (the same type as the fifth) and slimes in it. It has 15\~20 zombies and 20\~30 slimes in it.

The seventh wave has zombies (still the same type) and skeletons in it. It has 20\~25 zombies and 10\~15 skeletons in it.

The eigth wave only has zombies, only Common cards and mid weapons. It has 15\~20 zombies.

The ninth wave has zombies (same as previous wave), skeletons and slimes in it. It has 10\~15 zombies and 8\~12 skeletons and 15\~20 slimes in it.

The tenth wave has zombies (hand tailored buffs and attributes), skeletons and slimes. It has a predefined set of zombies, skeletons and slimes in it.

From the tenth wave upwards it has a random amount zombies (with any combination of buffs and weapons), skeletons and slimes that grows exponentially.
