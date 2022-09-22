# Light
<div align="center">
<img src="https://raw.githubusercontent.com/Light7734/Homepage/main/static/icons/light-project-icon.svg"/>

An idiot admires complexity, a genius admires simplicity.
</div>

 [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg?style=for-the-badge)](https://www.gnu.org/licenses/gpl-3.0)
 [![Conventional Commits](https://img.shields.io/badge/Conventional%20Commits-1.0.0-%23FE5196?logo=conventionalcommits&logoColor=white&style=for-the-badge)](https://conventionalcommits.org)

## What's all this?
Light Engine is a data-driven and highly-modularized game engine written in modern C++
  
## Development Guideline 
[![Trello Board](https://img.shields.io/badge/TRELLO-white?style=for-the-badge&logo=trello&logoColor=e21a41)](https://trello.com/b/gwn3xVEi/light-engine)

**Commit syntax follows the** [***conventional commits specification***](https://www.conventionalcommits.org/en/v1.0.0/):

```
<type>[optional scope]: <description>

[optional body]

[optional footer(s)]
```

**Commit types:**
- **feat:** Changes that add a new feature
- **fix:** Changes that fix a bug
- **refactor:** Changes that neither fix a bug nor add a feature
- **style:** Code changes that don't affect the compiled code
- **docs:** Changes to the documentations
- **perf:** Changes that improve the code performance
- **asset:** Changes to the assets

**Branches:**
- **main:** main development branch

**Getting started:**
```
git clone --recurse-submodules 'git@github.com:light7734/light'
mkdir light/Build
cd light/Build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cmake --build . -j`nproc`
cd ..
```
