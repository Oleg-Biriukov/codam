from src.CLI import CLI
import fire


if __name__ == "__main__":
    fire.Fire(CLI(input_file="data/raw/vllm-0.10.1"))
