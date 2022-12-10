#pragma once

#include <string>

/**
 * @brief The core implementation of the neural Network
 * 		- For now it's a fully, forward connected network
 *
 */
struct Network
{
  /**
   * @brief Simply initializes the name member to the name of the project
   */
  Network();

  std::string name;
};
