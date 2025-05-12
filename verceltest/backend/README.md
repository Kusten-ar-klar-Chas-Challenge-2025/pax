# Chas Challenge 2025

## Projektgrupp 2 Extended

### Gruppmedlemmar:

**SUVX24:** Jennifer Gott, Sabina Stawbrink, Oscar Asserlund, Erik Dahl, Johan Modin  
**FMWX24:** Hannah Bärlin, Hanna Kindholm, Tova Hansen  
**FJSX24:** Alice Eriksson, Phithai Lai, Dennis Granheimer, Rhiannon Brönnimann

[Projektplan](https://github.com/Kusten-ar-klar-Chas-Challenge-2025/pax/blob/main/PROJEKTPLAN.md)

## How to start server

1. Download and install docker desktop
2. In folder pax run command docker build -t my-node-app .
3. run docker-compose up
4. Enjoy the api

# API Documentation

Base URL: `http://localhost:{PORT}` (default port: 13000)

---

## Authentication

No authentication required yet.

---

## Endpoints

### 1. Setup Database Table

**GET** `/setup`

Initializes the database by creating the `rooms` table if it does not exist.

- **Response:**

  - `200 OK`

    ```
    Setup completed
    ```

  - `500 Internal Server Error`
    ```
    Error setting up database
    ```

---

### 2. Create a Room

**POST** `/rooms`

Create a new room with the specified details.

- **Request Body (JSON):**

  | Field       | Type    | Required | Description                         |
  | ----------- | ------- | -------- | ----------------------------------- |
  | name        | string  | Yes      | Name of the room                    |
  | description | string  | No       | Description of the room             |
  | available   | boolean | No       | Availability status (default: true) |
  | air_quality | integer | No       | Air quality index (default: 0)      |
  | screen      | boolean | No       | Whether the room has a screen       |
  | floor       | integer | No       | Floor number                        |
  | chairs      | integer | No       | Number of chairs                    |
  | whiteboard  | boolean | No       | Whether the room has a whiteboard   |
  | projector   | boolean | No       | Whether the room has a projector    |

- **Response:**

  - `201 Created`

    ```json
    {
      "id": 1,
      "name": "Conference Room A",
      "description": "Spacious room with projector",
      "available": true,
      "air_quality": 5,
      "screen": true,
      "floor": 2,
      "chairs": 10,
      "whiteboard": false,
      "projector": true
    }
    ```

  - `500 Internal Server Error`
    ```
    Error creating room
    ```

- **Example Request:**

  ```bash
  curl -X POST http://localhost:13000/rooms \
    -H "Content-Type: application/json" \
    -d '{
      "name": "Conference Room A",
      "description": "Spacious room with projector",
      "available": true,
      "air_quality": 5,
      "screen": true,
      "floor": 2,
      "chairs": 10,
      "whiteboard": false,
      "projector": true
    }'
  ```

### 3. Update a Room

**PUT** `/rooms/{id}`

Update the details of an existing room by its ID.

- **Path Parameters:**

  | Parameter | Type   | Description                  |
  | --------- | ------ | ---------------------------- |
  | id        | string | The ID of the room to update |

- **Request Body (JSON):**

  | Field       | Type    | Required | Description                         |
  | ----------- | ------- | -------- | ----------------------------------- |
  | name        | string  | Yes      | Name of the room                    |
  | description | string  | No       | Description of the room             |
  | available   | boolean | No       | Availability status (default: true) |
  | air_quality | integer | No       | Air quality index (default: 0)      |
  | screen      | boolean | No       | Whether the room has a screen       |
  | floor       | integer | No       | Floor number                        |
  | chairs      | integer | No       | Number of chairs                    |
  | whiteboard  | boolean | No       | Whether the room has a whiteboard   |
  | projector   | boolean | No       | Whether the room has a projector    |

- **Response:**

  - `200 OK`

    ```json
    {
      "id": 1,
      "name": "Updated Room Name",
      "description": "Updated description",
      "available": false,
      "air_quality": 3,
      "screen": false,
      "floor": 1,
      "chairs": 8,
      "whiteboard": true,
      "projector": false
    }
    ```

  - `404 Not Found`

    ```json
    {
      "message": "Room not found"
    }
    ```

  - `500 Internal Server Error`

    ```json
    {
      "message": "Internal server error"
    }
    ```

- **Example Request:**

  ```bash
  curl -X PUT http://localhost:13000/rooms/1 \
    -H "Content-Type: application/json" \
    -d '{
      "name": "Updated Room Name",
      "description": "Updated description",
      "available": false,
      "air_quality": 3,
      "screen": false,
      "floor": 1,
      "chairs": 8,
      "whiteboard": true,
      "projector": false
    }'
  ```

### Get All Rooms

**GET** `/rooms`

Retrieve a list of all rooms.

- **Response:**

  - `200 OK`

    ```json
    [
      {
        "id": 1,
        "name": "Conference Room A",
        "description": "Spacious room with projector",
        "available": true,
        "air_quality": 5,
        "screen": true,
        "floor": 2,
        "chairs": 10,
        "whiteboard": false,
        "projector": true
      },
      {
        "id": 2,
        "name": "Meeting Room B",
        "description": "Small room with whiteboard",
        "available": false,
        "air_quality": 4,
        "screen": false,
        "floor": 1,
        "chairs": 6,
        "whiteboard": true,
        "projector": false
      }
    ]
    ```

  - `500 Internal Server Error`
    ```
    Internal Server Error
    ```

- **Example Request:**

  ```bash
  curl http://localhost:13000/rooms
  ```
