NAME = src
UV_R = uv run python
UV_S = uv sync
ARG := 

install:
	$(UV_S)

run: install
	$(UV_R) -m $(NAME) $(ARG)

debug: install
	$(UV_R) -m pdb src/main.py $(ARG)

clean:
	@rm -rf .mypy_cache
	@rm -rf __pycache__
	@rm -rf ./*/__pycache__
	@rm -rf ./*/*/__pycache__

lint:
	$(UV_R) -m flake8 . --exclude=llm_sdk
	$(UV_R) -m mypy . --warn-return-any --warn-unused-ignores --ignore-missing-imports --disallow-untyped-defs --check-untyped-defs
