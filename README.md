project is a C++ simulation system that models post-war reconstruction plans for settlements in SPLand. Each plan executes a strategy for building facilities—such as schools, parks, offices, and factories—that influence quality of life, economy, and sustainability.

The system is fully object-oriented, follows the Rule of Five, and manages dynamic memory with no leaks (validated using Valgrind). It supports multiple policies, command-driven interactions, backups, and detailed logging.

Features:

Parse configuration files defining:

settlements (city/village/metropolis)

facility types and their contributions

initial plans with strategies

Run step-based simulation that:

selects next facility to build based on policy

tracks construction progress over time

updates settlement scores dynamically

Support multiple strategies:

Naive

Balanced

Economy-focused

Sustainability-focused

Action-based user interface:

plan — create a plan

step — advance time units

facility / settlement — add new entities

planStatus — view progress

changePolicy — modify selection strategy

backup / restore — snapshot simulation

log & close

 Project Structure:
├── include/
├── src/
├── bin/
├── makefile
└── config.txt

 Compilation & Run:
make
./bin/simulation <config_file>


Example:

./bin/simulation examples/config.txt

 Example Commands:
plan Kfar_SPL eco
step 3
changePolicy 1 bal
facility market 1 5 3 5 1
log
close

 Concepts Used:

Object-oriented design (multiple classes & hierarchies)

Rule of Five & dynamic memory handling

Inheritance + polymorphism for strategies

STL containers & custom resource ownership

Clean build system using Makefile

Command parsing & stateful simulation engine

 Testing:

Memory validation:

valgrind --leak-check=full --show-reachable=yes ./bin/simulation config.txt


Expected output:

All heap blocks were freed -- no leaks are possible
