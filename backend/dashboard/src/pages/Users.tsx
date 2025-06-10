// import {
//   Card,
//   CardContent,
//   CardDescription,
//   CardFooter,
//   CardHeader,
//   CardTitle,
// } from "@/components/Card";
import { User } from "../types/user";
import Button from "@/components/Button";
import { useEffect, useState } from "react";
import CreateUserModal from "@/components/CreateUserModal";
import UserCard from "@/components/UserCard";

const Users: React.FC = () => {
  const API_BASE_URL = "http://localhost:13000";
  const [users, setUsers] = useState<User[]>([]);

  // modal state
  const [isModalOpen, setIsModalOpen] = useState(false);
  const [editingUser, setEditingUser] = useState<User | null>(null);

  // fetch users from API
  useEffect(() => {
    const fetchData = async () => {
      try {
        const res = await fetch(`${API_BASE_URL}/users`);
        const data = await res.json();
        console.log("Fetched users:", data);
        setUsers(Array.isArray(data) ? data : data.users);
        console.log(data);
        setUsers(data);
      } catch (error) {
        console.error("Error fetching data:", error);
      }
    };
    fetchData();
  }, []);

  console.log(users);

  // create new user 
  const handleCreateUser = async (userData: Omit<User, "id">) => {
    try {
      const res = await fetch(`${API_BASE_URL}/users`, {
         method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify(userData),
      });
        if (!res.ok) throw new Error("Failed to create user");
        const newUser = await res.json();
        setUsers((prev) => [...prev, newUser]);
        setIsModalOpen(false);
    } catch (error) {
      console.error("Error creating user:", error);
    }
  };

  // update an existing user
  const handleUpdateUser = async (user: User) => {
  try {
      const res = await fetch(`${API_BASE_URL}/users/${user.id}`, {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(user),
      });
      if (!res.ok) throw new Error("Failed to update user");
      const updatedUser = await res.json();
      setUsers((prev) => prev.map((u) => (u.id === updatedUser.id ? updatedUser : u)));
    } catch (error) {
      console.error("Error updating user:", error);
    } finally {
      setEditingUser(null);
      setIsModalOpen(false);
    }
  };

  // delete a user
  const handleDeleteUser = async (id: number) => {
     if (!window.confirm("Are you sure you want to delete this user?")) return;
    try {
      const res = await fetch(`${API_BASE_URL}/users/${id}`, { method: "DELETE" });
      if (!res.ok) throw new Error("Failed to delete user");
      setUsers((prev) => prev.filter((user) => user.id !== id));
    } catch (error) {
      console.error("Error deleting user:", error);
    }
  };

  // mock data
  // const mockUsers: User[] = [
  //   {
  //     id: 1,
  //     name: "Alice",
  //     surname: "Andersson",
  //     email: "alice@example.com",
  //     password: "password123",
  //     role: "admin",
  //   },
  //   {
  //     id: 2,
  //     name: "Bob",
  //     surname: "Berg",
  //     email: "bob@example.com",
  //     password: "securepass",
  //     role: "user",
  //   },
  //   {
  //     id: 3,
  //     name: "Charlie",
  //     surname: "Carlsson",
  //     email: "charlie@example.com",
  //     password: "charliepw",
  //     role: "moderator",
  //   },
  //   {
  //     id: 4,
  //     name: "Diana",
  //     surname: "Dahl",
  //     email: "diana@example.com",
  //     password: "dianapw",
  //     role: "user",
  //   },
  // ];

  return (
    <main className="flex flex-col justify-center p-6 bg-gray-300 gap-4 text-black w-full h-screen overflow-auto">
      <section className="flex gap-4">
        <h1 className="text-2xl font-bold">Users</h1>
        <Button onClick={() => setIsModalOpen(true)}>Add User</Button>
      </section>
      <section className="flex-grow gap-4 flex flex-wrap">
        {users.length === 0 ? (
          <p className="text-gray-500">No users yet. Add a new user!</p>
        ) : (
          users.map((user) => (
            <UserCard
              key={user.id}
              user={user}
              onEdit={(u) => {
                setEditingUser(u);
                setIsModalOpen(true);
              }}
              onDelete={handleDeleteUser}
            />
          ))
        )}
      </section>
      <CreateUserModal
        isOpen={isModalOpen}
        onClose={() => {
          setIsModalOpen(false);
          setEditingUser(null);
        }}
        onCreate={handleCreateUser}
        onEdit={handleUpdateUser}
        userToEdit={editingUser || undefined}
      />
    </main>
  );
};

export default Users;
