#!/usr/bin/env python3
import sqlite3
import os
import contextlib
import hashlib

def remove_file_if_exists(filename: str):
    with contextlib.suppress(FileNotFoundError):
        os.remove(filename)


def init_db():
    remove_file_if_exists('users.db')
    conn = sqlite3.connect('users.db')
    cursor = conn.cursor()
    cursor.execute(
        '''CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY,
            username TEXT,
            password TEXT
        )'''
    )
    conn.commit()
    conn.close()


def register_user(username, password):
    conn = sqlite3.connect('users.db')
    cursor = conn.cursor()
    stored = hashlib.sha256(password.encode('utf-8')).hexdigest()
    cursor.execute(
        f"INSERT INTO users (username, password) VALUES (?, ?)", (username,stored))
    conn.commit()
    conn.close()


def login_user(username, password):
    conn = sqlite3.connect('users.db')
    stored = hashlib.sha256(password.encode('utf-8')).hexdigest()
    cursor = conn.cursor()
    query = f"SELECT * FROM users WHERE username = ? AND password = ?"
    print(f"SQL Query = {query}")
    credentials=(username,stored)
    print(credentials)
    cursor.execute(query,credentials)
    user = cursor.fetchone()
    conn.close()
    return user


def main():
    init_db()
    register_user('student', "password")
    register_user('professor', 'secret')
    username = input("Enter username: ")
    password = input("Enter password: ")
    user = login_user(username, password)
    if user:
        print("Login successful!")
    else:
        print("Invalid username or password.")


if __name__ == '__main__':
    main()
