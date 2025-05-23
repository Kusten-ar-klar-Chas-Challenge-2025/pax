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
import UserModal from "@/components/UserModal";

const Users: React.FC = () => {
  const API_BASE_URL = "http://localhost:13000";
  const [users, setUsers] = useState<User[]>([]);
  const [isModalOpen, setIsModalOpen] = useState(false);
  const [editingUser, setEditingUser] = useState<User | undefined>(undefined);

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
    <main className="flex overflow-auto flex-col gap-4 justify-center p-6 w-full h-screen text-black bg-gray-300">
      <section className="flex gap-4">
        <h1 className="text-2xl font-bold">Users</h1>
        <Button
          onClick={() => {
            setEditingUser(undefined);
            setIsModalOpen(true);
          }}
        >
          Add User
        </Button>
      </section>
      <section className="flex flex-grow gap-4">
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
              <Button
                onClick={() => {
                  setEditingUser(user);
                  setIsModalOpen(true);
                }}
              >
                Update
              </Button>
              <Button className="bg-red-500" onClick={async () => {
                   try {
                    const res = await fetch("http://localhost:13000/users", {
                      method: "DELETE",
                      headers: { "Content-Type": "application/json" },
                      body: JSON.stringify(user.id),
                    });
                    if (!res.ok) throw new Error("Kunde inte ta bort användaren");
                    // Optionally handle response here
                  } catch (error) {
                    console.error("Fel vid delete", error);
                  }
               }}>Delete</Button>
            </CardFooter>
          </Card>
        ))}
      </section>
      <UserModal
        isOpen={isModalOpen}
        onClose={() => setIsModalOpen(false)}
        user={editingUser}
      />
    </main>
  );
};
//ebba was here
export default Users;
