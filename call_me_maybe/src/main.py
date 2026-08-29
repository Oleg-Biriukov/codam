from llm_sdk.llm_sdk import Small_LLM_Model


def main():
    model = Small_LLM_Model()
    print(list(model.encode("What is python ?")))
    

if __name__ == "__main__":
    main()
