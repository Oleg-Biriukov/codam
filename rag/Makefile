NAME = src
PATH_ENV = /sgoinfre/obirukov/uv_env/.venv
UV_R = uv run python3
UV_S = uv sync
QUERY = "What determines whether vLLM's sampler returns Pythonized results or deferred Pythonization arguments?"
# ARG := index --max_chunk_size 2000
# ARG := search --query $(QUERY) --k 10
# ARG := search_dataset --dataset_path "datasets_public/public/UnansweredQuestions/dataset_docs_public.json" --k 10 --save_directory "./"
# ARG := answer --query $(QUERY) --k 10
ARG := answer_dataset --student_search_results_path "./dataset_docs_public.json" --save_directory "./data/datasets/"



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
