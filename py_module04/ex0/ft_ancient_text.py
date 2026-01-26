if __name__ == "__main__":
    '''Trying to open file via open() and read data for there.'''
    name = "ancient_fragment.txt"
    print("=== CYBER ARCHIVES - DATA RECOVERY SYSTEM ===\n")
    try:
        print("Accessing Storage Vault:", name)
        anc_fr = open(name, "r")
        print("Connection established...\n")
        print(f"RECOVERED DATA:\n{anc_fr.read()}")
        print("\nData recovery complete. Storage unit disconnected.")
        anc_fr.close()
    except Exception:
        print("ERROR:Storage vault not found.")
