if __name__ == "__main__":
    '''same as previos ex, but on the top we adding try/except
    method to avoid any file related errors'''
    text = "\n[CLASSIFIED] New security protocols archived"
    print('''=== CYBER ARCHIVES - CRISIS RESPONSE SYSTEM ===\n
CRISIS ALERT: Attempting access to 'lost_archive.txt'...''')
    try:
        with open("lost_archive.txt", "r") as data:
            d = data.read()
            print(f'SUCCESS: Archive recovered - {d}')
    except Exception as e:
        if type(e).__name__ == 'FileNotFoundError':
            print('RESPONSE: Archive not found in storage matrix')
        elif type(e).__name__ == 'PermissionError':
            print('RESPONSE: Security protocols deny access')
        else:
            print('RESPONSE: unknown error happened')
        print("STATUS: Crisis handled, security maintained")
    print("CRISIS ALERT: Attempting access to 'classified_vault.txt'...")
    try:
        with open("classified_vault.txt", "w") as data:
            data.write(text)
            print(f"Inscribing preservation data...\n{text}\n")
            print("Data inscription complete")
        print('''Vault automatically sealed upon completion
    All vault operations completed with maximum security.''')
    except Exception as e:
        if type(e).__name__ == 'FileNotFoundError':
            print('RESPONSE: Archive not found in storage matrix')
        elif type(e).__name__ == 'PermissionError':
            print('RESPONSE: Security protocols deny access')
        else:
            print('RESPONSE: unknown error happened')
        print("STATUS: Crisis handled, security maintained")
