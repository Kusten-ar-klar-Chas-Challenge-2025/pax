import React, { FC, useState, useEffect } from "react";
import Button from "./Button";
// import Auth from "@/pages/Auth";

// const [password, setPassword] = useState("");

// User type with role as string
type User = {
    id: number;
    name: string;
    surname: string;
    email: string;
    password: string;
    role: string; // e.g. "admin", "user", "moderator"
};

type CreateUserModalProps = {
    isOpen: boolean;
    onClose: () => void;
    onCreate: (user: Omit<User, "id">) => void;
    onEdit?: (user: User) => void;
    userToEdit?: User;
};

const CreateUserModal: FC<CreateUserModalProps> = ({ isOpen, onClose, onCreate, onEdit, userToEdit }) => {
    const [form, setForm] = useState<Omit<User, "id">>({
        name: "",
        surname: "",
        email: "",
        password: "",
        role: "user",
    });

    useEffect(() => {
        if (userToEdit) {
            const { id, ...rest } = userToEdit;
            setForm(rest);
        } else {
            setForm({
                name: "",
                surname: "",
                email: "",
                password: "",
                role: "user",
            });
        }
    }, [userToEdit]);

    const handleChange = (e: React.ChangeEvent<HTMLInputElement | HTMLSelectElement>) => {
        const { name, value } = e.target;
        setForm((prev) => ({
            ...prev,
            [name]: value,
        }));
    };

const handleSubmit = async () => {
    if (userToEdit && onEdit) {
        const user: User = {
            id: userToEdit.id,
            ...form,
        };
        onEdit(user);
        onClose();
        return;
    }

    try {
        const res = await fetch("http://localhost:13000/auth/register", {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
            },
            body: JSON.stringify(form),
        });

        if (!res.ok) {
            const errorData = await res.json();
            throw new Error(errorData.message || "Failed to create user");
        }

        const data = await res.json();
        console.log("User created:", data.user);
        onClose();
    } catch (error) {
        console.error("Error creating user:", error);
    }
};



    if (!isOpen) return null;

    return (
        <div className="fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-50">
            <div className="bg-white rounded-2xl shadow-lg p-6 w-full max-w-md space-y-4">
                <h2 className="text-xl font-semibold">
                    {userToEdit ? "Redigera användare" : "Skapa ny användare"}
                </h2>
                {/* Name */}
                <div className="space-y-2">
                    <label htmlFor="name" className="block text-sm font-medium text-gray-700">Förnamn</label>
                    <input
                        id="name"
                        className="w-full border rounded p-2"
                        placeholder="Förnamn"
                        name="name"
                        value={form.name}
                        onChange={handleChange}
                    />
                </div>
                {/* Surname */}
                <div className="space-y-2">
                    <label htmlFor="surname" className="block text-sm font-medium text-gray-700">Efternamn</label>
                    <input
                        id="surname"
                        className="w-full border rounded p-2"
                        placeholder="Efternamn"
                        name="surname"
                        value={form.surname}
                        onChange={handleChange}
                    />
                </div>
                {/* Email */}
                <div className="space-y-2">
                    <label htmlFor="email" className="block text-sm font-medium text-gray-700">E-post</label>
                    <input
                        id="email"
                        type="email"
                        className="w-full border rounded p-2"
                        placeholder="E-post"
                        name="email"
                        value={form.email}
                        onChange={handleChange}
                    />
                </div>
                {/* Password */}
                <div className="space-y-2">
                    <label htmlFor="password" className="block text-sm font-medium text-gray-700">Lösenord</label>
                    <input
                        id="password"
                        type="password"
                        className="w-full border rounded p-2"
                        placeholder="Lösenord"
                        name="password"
                        value={form.password}
                        onChange={handleChange}
                    />
                </div>
                {/* Role */}
                <div className="space-y-2">
                    <label htmlFor="role" className="block text-sm font-medium text-gray-700">Roll</label>
                    <select
                        id="role"
                        className="w-full border rounded p-2"
                        name="role"
                        value={form.role}
                        onChange={handleChange}
                    >
                        <option value="user">User</option>
                        <option value="admin">Admin</option>
                        <option value="moderator">Moderator</option>
                    </select>
                </div>
                {/* Buttons */}
                <div className="flex justify-end space-x-2">
                    <Button variant="outline" onClick={onClose}>Avbryt</Button>
                    <Button onClick={handleSubmit}>{userToEdit ? "Spara ändringar" : "Skapa"}</Button>
                </div>
            </div>
        </div>
    );
};

export default CreateUserModal;