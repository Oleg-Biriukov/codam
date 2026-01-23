text = "\n[CLASSIFIED] New security protocols archived"
print('''Initiating secure vault access...
Vault connection established with failsafe protocols\n''')
try:
    with open("classified_data.txt", "r") as data:
        print(data.read())
    print("SECURE PRESERVATION:")
    with open("classified_data.txt", "a") as data:
        data.write(text)
        print(text)
    print('''Vault automatically sealed upon completion
All vault operations completed with maximum security.''')
except Exception:
    print("ERROR:Storage vault not found.")

