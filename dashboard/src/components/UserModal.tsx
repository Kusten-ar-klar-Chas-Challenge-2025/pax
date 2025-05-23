import React, { FC, useEffect, useState } from "react";
import Modal from "./Modal";
import Button from "./Button";
import { User } from "@/types/user";

type UserModalProps = {
  isOpen: boolean;
  onClose: () => void;
  user?: User;
};

const UserModal: FC<UserModalProps> = ({ isOpen, onClose, user }) => {
  const [form, setForm] = useState<Omit<User, "id">>({
    name: "",
    email: "",
    role: "",
    surname: "",
    password: "",
  });

  useEffect(() => {
    if (user) {
      const { id, ...rest } = user;
      setForm(rest);
    } else {
      // Reset form if no user is being edited
      setForm({
        name: "",
        email: "",
        role: "",
        surname: "",
        password: "",
      });
    }
  }, [user]);

  if (!isOpen) return null; // Om modalen inte är öppen, rendera ingenting

  const handleChange = (
    e: React.ChangeEvent<HTMLInputElement | HTMLTextAreaElement>
  ) => {
    const { name, value, type, checked } = e.target;
    setForm((prev) => ({
      ...prev,
      [name]:
        type === "checkbox"
          ? checked
          : type === "number"
          ? Number(value)
          : value,
    }));
  };

  const handleSubmit = () => {
    if (user) {
      onEdit(user);
    } else if (onCreate) {
      onCreate(form);
    }
    onClose(); // Stänger modalen efter skapandet
  };

  const onEdit = async (user: User) => {
    try {
      const res = await fetch(`http://localhost:13000/users/${user.id}`, {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(user),
      });
      if (!res.ok) throw new Error("Kunde inte uppdatera användaren");
      // Optionally handle response here
    } catch (error) {
      console.error("Fel vid uppdatering", error);
    }
  };
  const onCreate = async (user: User) => {
    try {
      const res = await fetch("http://localhost:13000/users/", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(user),
      });
      if (!res.ok) throw new Error("Kunde inte skapa användaren");
      // Optionally handle response here
    } catch (error) {
      console.error("Fel vid skapande", error);
    }
  };

  return (
    <Modal>
      <div className="p-6 space-y-4 w-full max-w-md bg-white rounded-2xl shadow-lg">
        <h2 className="text-xl font-semibold">
          {user ? "Redigera användare" : "Skapa ny användare"}
        </h2>

        {/* Namn */}
        <div className="space-y-2">
          <label
            htmlFor="name"
            className="block text-sm font-medium text-gray-700"
          >
            Namn
          </label>
          <input
            id="name"
            className="p-2 w-full rounded border"
            placeholder="Namn"
            name="name"
            value={form.name}
            onChange={handleChange}
          />
        </div>
        {/* Efternamn */}
        <div className="space-y-2">
          <label
            htmlFor="surname"
            className="block text-sm font-medium text-gray-700"
          >
            Namn
          </label>
          <input
            id="surname"
            className="p-2 w-full rounded border"
            placeholder="Efternamn"
            name="surname"
            value={form.surname}
            onChange={handleChange}
          />
        </div>

        {/* E-post */}
        <div className="space-y-2">
          <label
            htmlFor="email"
            className="block text-sm font-medium text-gray-700"
          >
            E-post
          </label>
          <input
            id="email"
            className="p-2 w-full rounded border"
            placeholder="E-post"
            name="email"
            value={form.email}
            onChange={handleChange}
          />
        </div>
        <div className="">
          <div className="space-y-2">
            <label
              htmlFor="role"
              className="block text-sm font-medium text-gray-700"
            >
              Roll
            </label>
            <input
              id="role"
              type="text"
              className="p-2 rounded border"
              placeholder="Roll"
              name="role"
              value={form.role}
              onChange={handleChange}
            />
          </div>
        </div>

        {/* Knapp för att skicka formuläret */}
        <div className="flex justify-end space-x-2">
          <Button variant="outline" onClick={onClose}>
            Avbryt
          </Button>
          <Button onClick={handleSubmit}>
            {user ? "Spara ändringar" : "Skapa"}
          </Button>
        </div>
      </div>
    </Modal>
  );
};

export default UserModal;
