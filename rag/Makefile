NAME = src
PATH_ENV = /sgoinfre/obirukov/uv_env/.venv
UV_R = uv run python3
UV_S = uv sync
QUERY = "What are the default values for FP8_MIN and FP8_MAX constants in vLLM's triton_flash_attention module?"
ARG := index --max_chunk_size 2000
# ARG := search --query $(QUERY) --k 10
# ARG := answer --query $(QUERY) --k 10



export UV_PROJECT_ENVIRONMENT = $(PATH_ENV)

install:
	$(UV_S)

run: 
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
