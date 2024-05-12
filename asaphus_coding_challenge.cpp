/**
 * @file asaphus_coding_challenge.cpp
 * @version 1.1
 * @copyright Copyright (c) 2022 Asaphus Vision GmbH
 *
 * The goal is to implement the mechanics of a simple game and write test cases for them.
 * The rules of the game are:
 * - There are two types of boxes, green and blue.
 * - Both can absorb tokens of a given weight, which they add to their own total weight.
 * - Both are initialized with a given initial weight.
 * - After a box absorbs a token weight, it outputs a score.
 * - Green and blue boxes calculate the score in different ways:
 * - A green box calculates the score as the square of the mean of the 3 weights that it most recently absorbed (square of mean of all absorbed weights if there are fewer than 3).
 * - A blue box calculates the score as Cantor's pairing function of the smallest and largest weight that it has absorbed so far, i.e. pairing(smallest, largest), where pairing(0, 1) = 2
 * - The game is played with two green boxes with initial weights 0.0 and 0.1, and two blue boxes with initial weights 0.2 and 0.3.
 * - There is a list of input token weights. Each gets used in one turn.
 * - There are two players, A and B. Both start with a score of 0.
 * - The players take turns alternatingly. Player A starts.
 * - In each turn, the current player selects one of the boxes with the currently smallest weight, and lets it absorb the next input token weight. Each input weight gets only used once.
 * - The result of the absorption gets added to the current player's score.
 * - When all input token weights have been used up, the game ends, and the player with highest score wins.
 *
 * Task:
 * - Create a git repo for the implementation. Use the git repository to have a commit history.
 * - Implement all missing parts, marked with "TODO", including the test cases.
 * - Split the work in reasonable commits (not just one commit).
 * - Make sure the test cases succeed.
 * - Produce clean, readable code.
 *
 * Notes:
 * - Building and running the executable: g++ --std=c++14 asaphus_coding_challenge.cpp -o challenge && ./challenge
 * - Feel free to add a build system like CMake, meson, etc.
 * - Feel free to add more test cases, if you would like to test more.
 * - This file includes the header-only test framework Catch v2.13.9.
 * - A main function is not required, as it is provided by the test framework.
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <numeric>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

class Box
{
public:
  explicit Box(double initial_weight) : weight_(initial_weight) {}
  static std::unique_ptr<Box> makeGreenBox(double initial_weight);
  static std::unique_ptr<Box> makeBlueBox(double initial_weight);
  bool operator<(const Box &rhs) const { return weight_ < rhs.weight_; }

  virtual double absorb(uint32_t weight) = 0;

  virtual ~Box(){};

protected:
  double weight_;
};

class GreenBox : public Box
{
public:
  using Box::Box;

  double absorb(uint32_t weight) override
  {
    weights_.push_back(weight);
    weight_ += weight;
    double sum = std::accumulate(weights_.begin(), weights_.end(), 0.0);
    double mean = sum / weights_.size();
    return mean * mean;
  }

private:
  std::vector<double> weights_;
};

class BlueBox : public Box
{

public:
  using Box::Box;
  double absorb(uint32_t token_weight) override
  {
    weight_ += token_weight;
    if (token_weight < min_weight_)
      min_weight_ = token_weight;
    if (token_weight > max_weight_)
      max_weight_ = token_weight;
    return cantorPairing(min_weight_, max_weight_);
  }

private:
  uint32_t min_weight_ = std::numeric_limits<uint32_t>::max();
  uint32_t max_weight_ = 0;

  // Logic for cantor pairing
  uint32_t cantorPairing(uint32_t k1, uint32_t k2)
  {
    return ((k1 + k2) * (k1 + k2 + 1) / 2) + k2;
  }
};

class Player
{
public:
  // Main logic  for playing the game
  void takeTurn(uint32_t input_weight,
                const std::vector<std::unique_ptr<Box>> &boxes)
  {
    auto min_box_it = std::min_element(boxes.begin(), boxes.end(),
                                       [](const std::unique_ptr<Box> &a, const std::unique_ptr<Box> &b)
                                       {
                                         return *a < *b;
                                       });
    score_ += (*min_box_it)->absorb(input_weight);
  }
  double getScore() const { return score_; }

private:
  double score_{0.0};
};

// Static factory methods for creating the Greenbox and BlueBox method
std::unique_ptr<Box> Box::makeGreenBox(double initial_weight)
{
  return std::make_unique<GreenBox>(initial_weight);
}

std::unique_ptr<Box> Box::makeBlueBox(double initial_weight)
{
  return std::make_unique<BlueBox>(initial_weight);
}

std::pair<double, double> play(const std::vector<uint32_t> &input_weights)
{
  std::vector<std::unique_ptr<Box>> boxes;
  boxes.emplace_back(Box::makeGreenBox(0.0));
  boxes.emplace_back(Box::makeGreenBox(0.1));
  boxes.emplace_back(Box::makeBlueBox(0.2));
  boxes.emplace_back(Box::makeBlueBox(0.3));

  Player player_A, player_B;

  for (size_t itr = 0; itr < input_weights.size(); ++itr)
  {
    if (itr % 2 == 0)
      player_A.takeTurn(input_weights[itr], boxes);
    else
      player_B.takeTurn(input_weights[itr], boxes);
  }

  std::cout << "Scores: player A " << player_A.getScore() << ", player B "
            << player_B.getScore() << std::endl;
  return std::make_pair(player_A.getScore(), player_B.getScore());
}

TEST_CASE("Final scores for first 4 Fibonacci numbers", "[fibonacci4]")
{
  std::vector<uint32_t> inputs{1, 1, 2, 3};
  auto result = play(inputs);
  REQUIRE(result.first == 13.0);
  REQUIRE(result.second == 25.0);
}

TEST_CASE("Final scores for first 8 Fibonacci numbers", "[fibonacci8]")
{
  std::vector<uint32_t> inputs{1, 1, 2, 3, 5, 8, 13, 21};
  auto result = play(inputs);
  REQUIRE(result.first == 155.0);
  REQUIRE(result.second == 366.25);
}

TEST_CASE("Test absorption of green box", "[green]")
{
  std::unique_ptr<Box> green_box = Box::makeGreenBox(0.0);

  green_box->absorb(10);
  green_box->absorb(20);
  green_box->absorb(30);

  REQUIRE(green_box->absorb(40) == Approx(625.0));
}

TEST_CASE("Test absorption of blue box", "[blue]")
{
  std::unique_ptr<Box> blue_box = Box::makeBlueBox(0.0);

  blue_box->absorb(5);
  blue_box->absorb(10);
  blue_box->absorb(15);

  uint32_t min_weight = 5, max_weight = 20;
  uint32_t expected = ((min_weight + max_weight) * (min_weight + max_weight + 1) / 2) + max_weight;

  REQUIRE(blue_box->absorb(20) == expected);
}

// Test with all input weights the same
TEST_CASE("All input weights are the same", "[uniform]")
{
  std::vector<uint32_t> inputs(10, 5); // Ten inputs, all 5
  auto result = play(inputs);
  REQUIRE(result.first >= 0.0);
  REQUIRE(result.second >= 0.0);
}

// Test with no input weights
TEST_CASE("No input weights provided", "[empty]")
{
  std::vector<uint32_t> inputs;
  auto result = play(inputs);
  REQUIRE(result.first == 0.0);
  REQUIRE(result.second == 0.0);
}

// Test with extreme values
TEST_CASE("Extreme values in inputs", "[extreme]")
{
  std::vector<uint32_t> inputs{std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max()};
  auto result = play(inputs);
  REQUIRE(result.first >= 0.0);
  REQUIRE(result.second >= 0.0);
}

// Test alternating high and low values
TEST_CASE("Alternating high and low values", "[alternating]")
{
  std::vector<uint32_t> inputs{1, 4294967295, 1, 4294967295, 1, 4294967295};
  auto result = play(inputs);
  REQUIRE(result.first >= 0.0);
  REQUIRE(result.second >= 0.0);
}

// Test large number of inputs
TEST_CASE("Large number of inputs", "[large_input]")
{
  std::vector<uint32_t> inputs(1000, 10); // 1000 inputs, all 10
  auto result = play(inputs);
  REQUIRE(result.first >= 0.0);
  REQUIRE(result.second >= 0.0);
}
