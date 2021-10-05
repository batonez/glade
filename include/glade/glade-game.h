#pragma once

#include <glade/State.h>
#include <memory>

class FileManager;

struct InitialGameData {
  std::unique_ptr<State> initialState;
};

void glade_game_init(InitialGameData *game_init_data, FileManager *file_manager);

