NAME = src.main
UV_R = uv run python
UV_S = uv sync
ENV_DIR = /home/obirukov/sgoinfre/uv_env
TOKEN = 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
ARG := 

export UV_PROJECT_ENVIRONMENT := $(ENV_DIR)
export HF_HUB_DISABLE_IMPLICIT_TOKEN_WARNING := 1
export HF_TOKEN := $(TOKEN)

install:
	$(UV_S)

run: install
	$(UV_R) -m $(NAME) $(ARG)

debug: install
	$(UV_R) -m pdb src/main.py $(ARG)

clean:
	@rm -rf __pycache__
	@rm -rf ./*/__pycache__
	@rm -rf ./*/*/__pycache__

lint:
	$(UV_R) -m flake8 . --exclude=llm_sdk
	$(UV_R) -m mypy . --warn-return-any --warn-unused-ignores --ignore-missing-imports --disallow-untyped-defs --check-untyped-defs
