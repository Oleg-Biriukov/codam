NAME = src.main
ARG =
UV_R = uv run python
UV_S = uv sync
ENV_DIR = /home/obirukov/sgoinfre/uv_env

export UV_PROJECT_ENVIRONMENT := $(ENV_DIR)
export HF_HUB_DISABLE_IMPLICIT_TOKEN_WARNING := 1

install:
	$(UV_S)

run: install
	$(UV_R) -m $(NAME) $(ARG)

debug: install
	$(UV_R) -m pdb $(NAME) $(ARG)

clean:
	@rm -rf __pycache__
	@rm -rf ./*/__pycache__
	@rm -rf ./*/*/__pycache__

lint:
	flake8 .
	mypy . --warn-return-any --warn-unused-ignores --ignore-missing-imports --disallow-untyped-defs --check-untyped-defs
