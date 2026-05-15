NAME := /tmp/venv
FILE := .
PYTHON := $(NAME)/bin/python
PIP := $(NAME)/bin/pip
VENV := $(NAME)/bin/activate

MAP := config.txt
MAP_DBG := config.txt

ifneq ($(filter run,$(MAKECMDGOALS)),)
	ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

	ifneq ($(ARGS),)
		MAP := $(firstword $(ARGS))
	endif
endif

ifneq ($(filter debug,$(MAKECMDGOALS)),)
	ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

	ifneq ($(ARGS),)
		MAP_DBG := $(firstword $(ARGS))
	endif
endif

$(VENV):
	@python3 -m venv $(NAME)

install: $(VENV)
	@$(PYTHON) -m pip install -r requirements.txt > /dev/null 2>&1

run: install
	@$(PYTHON) $(FILE) $(MAP)

debug: install
	@$(PYTHON) -m pdb __main__.py $(MAP_DBG)

clean:
	rm -rf *cache* .mypy*
	rm -rf */*cache* */.mypy*

lint: $(VENV)
	@$(PYTHON) -m mypy *.py || true
	@$(PYTHON) -m flake8 *.py */*.py || true

%:
	@: