if __name__ == "__main__":
    name = "new_discovery.txt"
    text = '''[ENTRY 001] New quantum algorithm discovered
[ENTRY 002] Efficiency increased by 347%
[ENTRY 003] Archived by Data Archivist trainee'''
    print("=== CYBER ARCHIVES - PRESERVATION SYSTEM ===\n")
    try:
        print("Initializing new storage unit:", name)
        anc_fr = open(name, "w")
        try:
            anc_fr.write(text)
            print("Connection established...\n")
            print(f"Inscribing preservation data...\n{text}")
            print(f'''Data inscription complete. Storage unit sealed.
Archive '{name}' ready for long-term preservation.''')
        except Exception:
            print("ERROR: Problem with inscribing data.")
        finally:
            anc_fr.close()
    except Exception:
        print("ERROR:Storage vault not found.")
