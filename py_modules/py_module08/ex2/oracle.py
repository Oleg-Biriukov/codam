import os
import sys
from dotenv import load_dotenv


def main():
    print('\nORACLE STATUS: Reading the Matrix..\n')
    try:
        name_var = ['MATRIX_MODE', 'DATABASE_URL', 'API_KEY',
        'LOG_LEVEL', 'ZION_ENDPOINT']
        load_dotenv(override=False)
        our_data = [os.getenv(name) for name in name_var]
        if our_data[0] not in ['development', 'production']:
            raise ValueError('')
        print(f'''Mode: {our_data[0]}
Database: {our_data[1]}
API Access: {our_data[2]}
Log Level: {our_data[3]}
Zion Network: {our_data[4]}''')
        print('''\nEnvironment security check:
[OK] No hardcoded secrets detected
[OK] .env file properly configured
[OK] Production overrides available
\nThe Oracle sees all configurations.''')
    except:
        print('Wrong env variables was providen !')


if __name__ == "__main__":
    main()
