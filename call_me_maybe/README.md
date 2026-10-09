*This project has been created as part of the 42 curriculum by obirukov.*

# call me maybe

## Description

This project implements a small function-calling pipeline for an LLM. The system reads a list of available functions, chooses the function that best matches a natural-language prompt, and generates a JSON object whose structure follows the function schema.

Instead of returning a direct answer such as "42", the program produces a structured call such as:

```json
{
  "prompt": "What is the sum of 40 and 2?",
  "name": "fn_add_numbers",
  "parameters": {
    "a": 40,
    "b": 2
  }
}
```

The goal is to show how a small model can be guided toward valid, machine-readable output using constrained decoding and schema-aware generation.

## Project architecture

The implementation is organized around a few core files:

- `src/__main__.py`: CLI entry point. It parses arguments (`--functions_definition`, `--input`, `--output`), loads JSON files, instantiates the generation workflow, and writes the final output.
- `src/state_machine.py`: Contains the state machine and the generation orchestration logic. It selects function names from allowed tokens, builds parameter patterns, and enforces token validity during generation.
- `src/definer_func.py`: Defines validation models for function definitions and user prompts through Pydantic. It contains the `FuncDefiner`, `UserPrompt`, and `Types` enum used to validate input and output structure.
- `src/error_handler.py`: Wraps critical functions to catch malformed files, validation errors, JSON decode errors, and unexpected failures with clear messages instead of crashing.
- `Makefile`: Provides project automation for install, run, debug, clean, and lint operations.

## Instructions

### Requirements

- Python 3.10+
- `uv`
- `pydantic`
- `numpy`
- `flake8`
- `mypy`

### Installation

```bash
uv sync
```

### Running the project

Default execution:

```bash
uv run python -m src
```

With explicit files:

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

## Algorithm explanation

The generation logic is based on token-by-token constrained decoding.

1. The function definitions are loaded from JSON and validated using Pydantic.
2. Each prompt is converted into a generation prompt by combining the user request with a function-selection context.
3. The model generates a function name from a set of valid function tokens rather than allowing free-form text generation.
4. Once the function is selected, the system builds an expected parameter pattern based on the function schema.
5. The state machine tracks the current generation phase:
   - selecting the function name
   - selecting parameter names
   - generating string, number, boolean, or list values
6. Before accepting a token, the system filters invalid candidates using the schema and the current state. Tokens that would break JSON structure or violate the expected type are rejected.
7. The output is reassembled into valid JSON and written to disk.

This is the core idea behind the project: instead of asking the LLM to generate JSON "by chance", the system restricts generation so that the output is always syntactically valid and type-consistent with the function definition.

## Design decisions

- Pydantic validation: Input and function definitions are validated early to avoid malformed schemas.
- State-based generation: The project uses explicit states to manage how the model is expected to produce names and values.
- Token masking: The implementation prefers constrained token selection over raw greedy generation to preserve structure.
- Graceful failure handling: File errors, JSON parse errors, and validation problems are caught and reported clearly.
- Clear command-line interface: The project supports custom input and output paths without changing the core logic.

## Performance analysis

This project is designed for reliability rather than unrestricted free-text generation. In practice, the constrained approach improves the output quality of small models by limiting invalid sequences.

The key advantages are:

- valid JSON output
- correct function selection from a known set
- proper argument typing
- robust handling of edge cases
- minimal dependence on fragile prompt-only behavior

The system is not fully solving all open-ended generation issues, but it is well suited for structured tool invocation under controlled schemas.

## Challenges faced

- Small models often produce malformed JSON when allowed to generate freely.
- Missing or invalid input files must not crash the program.
- Parameter types vary between strings, numbers, booleans, and lists, and the generator must honor each one.
- Escaping characters in strings and JSON syntax can break generation if not handled carefully.
- The pipeline must maintain both semantic correctness and structural correctness at the same time.

The state machine and token filtering are used to solve these issues by enforcing the expected generation sequence at each step.

## Testing strategy

The project should be validated with a combination of functional checks and edge-case testing:

1. Run the default input file:
   ```bash
   uv run python -m src
   ```
2. Check that the output file is valid JSON.
3. Verify that every object includes `prompt`, `name`, and `parameters`.
4. Confirm that the chosen function exists in the function definition file.
5. Confirm that parameters match the expected types exactly.
6. Test malformed JSON, missing files, empty strings, and unusual characters.

Example validation:

```bash
python -m json.tool data/output/function_calling_results.json
```

## Example usage

### Example 1

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

### Example 2

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

### Technical references

- Hugging Face tokenization and generation documentation
- JSON schema and structured-output practices
- Function calling and tool-use patterns in modern LLM systems
- Constrained decoding and grammar-guided generation resources

### AI usage

AI tools were used to support:

- understanding the function-calling workflow
- improving prompts and code reasoning
- validating edge cases and debugging ideas
- reviewing the structure of the generation pipeline

The final implementation was tested and adapted by the developer before being kept in the repository.

## Summary

This project demonstrates a practical implementation of function calling for a small LLM. The system chooses a function from a schema, generates values under type constraints, and writes valid JSON output using constrained decoding. This makes the approach much more reliable than directly prompting a small model to produce structured output without control.
