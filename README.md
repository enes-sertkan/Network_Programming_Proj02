# AuthenticationDB

=====================

## TEAMMEMBERS:

Enes Sertkan 1286651

Vasili Tsyganov 1266871

A database schema for managing user authentication and chat functionality.

## Overview

---

This project provides a database schema for storing user information, authentication data, and chat messages. The schema is designed to support a robust authentication system and a scalable chat platform.

## Features

---

- User authentication and authorization
- One-on-one chat messaging
- Chat room messaging
- User blocking and unblocking
- User friending and unfriending

## Database Schema

---

The database schema consists of the following tables:

- `Users`: stores information about each user, including their email, password hash, creation date, last login date, and whether they are active.
- `User Sessions`: stores information about each user's session, including the user ID, session token, creation date, and expiration date.
- `BlockedUsers`: stores information about which users have blocked other users.
- `User Friends`: stores information about which users are friends with each other.
- `ChatRooms`: stores information about each chat room, including the room name and creation date.
- `ChatRoomMembers`: stores information about which users are members of each chat room.
- `ChatRoomMessages`: stores individual chat room messages, including the chat room ID, sender ID, message text, and creation date.
- `OneOnOneChatMessages`: stores individual one-on-one chat messages, including the sender ID, recipient ID, message text, and creation date.

## Schemas

---

The database schema is organized into three schemas:

- `Auth`: for authentication-related tables and objects.
- `Chat`: for chat-related tables and objects.
- `Users`: for user-related tables and objects.

## Getting Started

---

To get started with this project, follow these steps:

1.  Create a new database named `AuthenticationDB`.
2.  Run the SQL script provided in the `AuthenticationDB_schema.sql` file to create the database schema.
3.  Use the schema to store user information, authentication data, and chat messages.

## Contributing

---

Contributions to this project are welcome. If you have any suggestions or bug reports, please submit an issue or pull request.
