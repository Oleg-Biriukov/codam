*This project has been created as part of the 42 curriculum by obirukov.*

# call me maybe

## Description

This project explores function calling in large language models (LLMs): instead of returning a plain natural-language answer, the system must choose the correct function and produce machine-readable arguments in valid JSON format.

The goal is to turn a prompt such as "What is the sum of 40 and 2?" into a structured call like:

```json
{
  "name": "fn_add_numbers",
  "parameters": {
    "a": 40,
    "b": 2
  }
}
```

In other words, the model is not asked to answer the question directly. It is asked to select the right tool and provide arguments with the correct types. This is a core pattern in modern AI systems, where models interact with external tools, APIs, executors, and structured workflows.

The project is built around constrained decoding: a small model such as Qwen/Qwen3-0.6B can produce fully valid JSON reliably when token generation is restricted to only syntactically and semantically valid outputs.

## Project goals

- Select the right function from a provided function catalog
- Extract arguments with the correct types
- Enforce a valid JSON output format
- Use constrained decoding instead of relying on prompt-only generation
- Gracefully handle invalid or missing files and malformed input
- Keep the implementation fast, robust, and reproducible

## Instructions

### Requirements

- Python 3.10 or later
- `uv` for dependency management
- `flake8` for linting
- `mypy` for static type checking
- `pydantic` for input validation
- `numpy` and `json` as allowed packages

### Installation

```bash
uv sync
```

### Execution

Run the project with the default files:

```bash
uv run python -m src
```

Run the project with explicit paths:

```bash
uv run python -m src \
  --functions_definition data/input/functions_definition.json \
  --input data/input/function_calling_tests.json \
  --output data/output/function_calling_results.json
```

### Makefile targets

```bash
make install
make run
make debug
make clean
make lint
```

The project also supports `lint-strict` as an optional stricter validation target.

## Repository layout

```text
.
├── data/
│   ├── input/
│   │   ├── function_calling_tests.json
│   │   └── functions_definition.json
│   └── output/
├── llm_sdk/
├── src/
├── Makefile
├── pyproject.toml
├── uv.lock
├── README.md
└── .gitignore
```

## Algorithm explanation

The implementation follows a constrained-decoding pipeline to generate structured function calls.

1. Input parsing
   - The system reads the list of available functions from `data/input/functions_definition.json`.
   - Each function definition contains its name, parameter schema, return type, and description.
   - The natural-language prompts are loaded from `data/input/function_calling_tests.json`.

2. LLM interaction
   - The project uses the provided `llm_sdk` wrapper.
   - The model exposes methods such as `get_logits_from_input_ids`, `get_path_to_vocab_file`, `encode`, and optionally `decode`.

3. Token-level generation
   - The prompt is tokenized into input IDs.
   - The model produces logits for the next token.
   - A valid function call must respect both JSON syntax and the target schema.

4. Constrained decoding
   - Invalid tokens are masked by setting their logit values to negative infinity.
   - This prevents the model from generating malformed JSON, wrong keys, invalid argument types, or invalid function names.
   - At each step, only tokens compatible with the grammar and schema remain eligible.

5. Schema enforcement
   - The system keeps track of the currently expected structure:
     - start of object
     - `name` field
     - `parameters` field
     - valid parameter names and types
   - Numeric fields remain numbers, strings remain strings, booleans remain booleans, and arrays/objects follow the schema.

6. Output writing
   - Once generation is complete, the result is validated and serialized to the output JSON file.
   - The program emits one object per prompt with exactly:
     - `prompt`
     - `name`
     - `parameters`

This design intentionally avoids relying on the model to "guess" valid JSON without constraints. Instead, structure is enforced at generation time, which dramatically improves reliability for small models.

## Design decisions

- Pydantic validation: Strong validation at model boundaries ensures that function definitions and prompt payloads are well-formed.
- Separation of responsibilities: Parsing, validation, generation, and writing are isolated into dedicated functions.
- File-based workflow: The project is built to work with JSON files in a clear and reproducible pipeline.
- Graceful failure handling: Invalid JSON inputs, missing files, and edge cases are handled without crashing the program.
- Compatibility with the provided SDK: The implementation respects the repository constraints and avoids private attributes from `llm_sdk`.
- Simplicity and portability: The code stays compatible with Python 3.10+, `uv`, and standard local tooling.

## Performance analysis

The implementation is designed to meet the project targets:

- Accuracy: 90%+ on function selection and argument extraction under constrained decoding
- Reliability: 100% valid JSON at the output stage when constraints are enforced correctly
- Speed: Process all prompt examples within a few minutes on standard hardware
- Robustness: malformed or missing files are reported clearly rather than crashing

Given the model size (Qwen/Qwen3-0.6B), raw unconstrained generation is unreliable. The constrained approach addresses this by steering generation toward valid output tokens, which is why small models can still achieve production-like reliability.

## Challenges faced

- Small models often produce invalid JSON or hallucinated fields.
- Schema drift between different function definitions can break generation.
- Token vocabularies require careful mapping between token IDs and literal string representations.
- Missing or malformed input files must not terminate the program unexpectedly.
- The model must not be allowed to pick a function by heuristics or hardcoded logic; selection must remain model-driven.

The solution addressed these issues by applying schema-aware constrained decoding, validating each result before writing it, and using explicit error handling for incomplete or broken inputs.

## Testing strategy

The project should be tested with a combination of validation and edge-case checks.

### Recommended checks

- Run the default examples in `data/input/function_calling_tests.json`
- Validate that the output file is valid JSON
- Verify that each object includes exactly `prompt`, `name`, and `parameters`
- Confirm that the selected function matches one of the definitions
- Check that parameter names and types match the schema exactly
- Test empty strings, large numbers, special characters, and ambiguous prompts
- Validate the behavior when input files are missing or corrupted

### Example validation workflow

```bash
uv run python -m src
python -m json.tool data/output/function_calling_results.json
```

Unit tests and custom regression checks may also be added during development to validate edge cases without submitting them for grading.

## Example usage

### Example 1: Add two numbers

Input prompt:

```json
{
  "prompt": "What is the sum of 2 and 3?"
}
```

Expected output:

```json
{
  "prompt": "What is the sum of 2 and 3?",
  "name": "fn_add_numbers",
  "parameters": {
    "a": 2.0,
    "b": 3.0
  }
}
```

### Example 2: Reverse a string

Input prompt:

```json
{
  "prompt": "Reverse the string 'hello'"
}
```

Expected output:

```json
{
  "prompt": "Reverse the string 'hello'",
  "name": "fn_reverse_string",
  "parameters": {
    "s": "hello"
  }
}
```

## Resources

### Relevant references

- Hugging Face Transformers documentation on tokenization and generation
- Model vocabulary and tokenizer usage
- JSON schema and structured output generation patterns
- Function-calling documentation for LLM tool use
- Articles on constrained decoding and structured generation for small language models

### AI usage

AI tools were used as a support mechanism during development for:

- generating and refining prompts
- explaining model behavior and structured-output patterns
- reviewing debugging ideas and validation approaches
- proposing edge-case tests and implementation refinements

All AI-generated content was reviewed, validated, and adapted with human understanding before being integrated into the project. This follows the project’s requirement to use AI responsibly and to keep final responsibility with the developer.

## Bonus part

Optional improvements that may be added later include:

- support for multiple LLM backends beyond Qwen/Qwen3-0.6B
- tokenizer reimplementation without direct `encode`/`decode` in main logic
- advanced error recovery and retry logic
- batching and caching to improve throughput
- extended nested argument support
- visual generation tracing and debugging tools

These features are optional and not required for a passing project, but they can significantly improve both quality and maintainability.

## Submission requirements reminder

The project must include:

- `src/` directory with the implementation
- `pyproject.toml` and `uv.lock`
- `llm_sdk/` directory with the provided SDK or a local copy
- `data/input/` with test files
- a root-level `README.md` with complete project documentation
- no generated output directory committed to the repository

## Summary

This project demonstrates that small LLMs can be used reliably for structured tool calling when generation is constrained by a schema and a valid JSON grammar. The result is a robust function-calling system suited to real-world automation, data extraction, and tool invocation workflows.
