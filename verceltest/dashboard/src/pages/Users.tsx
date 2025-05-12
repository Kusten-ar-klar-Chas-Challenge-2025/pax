import {
  Card,
  CardContent,
  CardDescription,
  CardFooter,
  CardHeader,
  CardTitle,
} from "@/components/Card";
import { User } from "../types/user";
import Button from "@/components/Button";
import { useEffect, useState } from "react";
const Users: React.FC = () => {
  const API_BASE_URL = "http://localhost:13000";
  const [users, setUsers] = useState<User[]>([]);

  useEffect(() => {
    const fetchData = async () => {
      try {
        const res = await fetch(`${API_BASE_URL}/users`);
        const data = await res.json();
        console.log(data);
        setUsers(data);
      } catch (error) {
        console.error("Error fetching data:", error);
      }
    };
    fetchData();
  }, []);
  console.log(users);
  const mockUsers: User[] = [
    {
      id: 1,
      name: "Alice",
      surname: "Andersson",
      email: "alice@example.com",
      password: "password123",
      role: "admin",
    },
    {
      id: 2,
      name: "Bob",
      surname: "Berg",
      email: "bob@example.com",
      password: "securepass",
      role: "user",
    },
    {
      id: 3,
      name: "Charlie",
      surname: "Carlsson",
      email: "charlie@example.com",
      password: "charliepw",
      role: "moderator",
    },
    {
      id: 4,
      name: "Diana",
      surname: "Dahl",
      email: "diana@example.com",
      password: "dianapw",
      role: "user",
    },
  ];

  return (
    <main className="flex flex-col justify-center p-6 bg-gray-300 gap-4 text-black w-full h-screen overflow-auto">
      <section className="flex gap-4">
        <h1 className="text-2xl font-bold">Users</h1>
        <Button onClick={() => {}}>Add User</Button>
      </section>
      <section className="flex-grow gap-4 flex">
        {mockUsers.map((user) => (
          <Card key={user.id} className="h-fit">
            <CardHeader>
              <CardTitle>
                {user.name} {user.surname}
              </CardTitle>
              <CardDescription>{user.email}</CardDescription>
            </CardHeader>
            <CardContent>
              <p> Role: {user.role}</p>
            </CardContent>
            <CardFooter className="flex gap-2">
              <Button onClick={() => {}}>Update</Button>
              <Button onClick={() => {}}>Delete</Button>
            </CardFooter>
          </Card>
        ))}
      </section>
    </main>
  );
};

export default Users;
