import { FC, useState, useEffect } from "react";
import Button from "./Button";

type Room = {
    id: number;
    name: string;
    description?: string;
    available: boolean;
    air_quality: number;
    screen: boolean;
    floor: number;
    chairs: number;
    whiteboard: boolean;
    projector: boolean;
};

type CreateRoomModalProps = {
    isOpen: boolean;
    onClose: () => void;
    onCreate: (room: Omit<Room, "id">) => void;
    onEdit?: (room: Room) => void;
    roomToEdit?: Room;
};

const CreateRoomModal: FC<CreateRoomModalProps> = ({ isOpen, onClose, onCreate, onEdit, roomToEdit, }) => {
    const [form, setForm] = useState<Omit<Room, "id">>({
        name: "",
        description: "",
        available: true,
        air_quality: 0,
        screen: false,
        floor: 0,
        chairs: 0,
        whiteboard: false,
        projector: false,
    });

    useEffect(() => {
        if (roomToEdit) {
          const { id, ...rest } = roomToEdit;
          setForm(rest);
        } else {
        // Reset form if no room is being edited
        setForm({
            name: "",
            description: "",
            available: true,
            air_quality: 0,
            screen: false,
            floor: 0,
            chairs: 0,
            whiteboard: false,
            projector: false,
        });
        }
      }, [roomToEdit]);

    const handleChange = (e: React.ChangeEvent<HTMLInputElement | HTMLTextAreaElement>) => {
        const { name, value, type, checked } = e.target;
        setForm((prev) => ({
            ...prev,
            [name]: type === "checkbox" ? checked : type === "number" ? Number(value) : value,
        }));
    };

    const handleSubmit = () => {
        const room: Room = {
            id: roomToEdit?.id ?? Date.now(), // reuse ID if editing
            ...form,
          };
        
          if (roomToEdit && onEdit) {
            onEdit(room);
          } else if (onCreate) {
            onCreate(room);
          }
        onClose(); // Stänger modalen efter skapandet
    };

    if (!isOpen) return null;  // Om modalen inte är öppen, rendera ingenting

    return (
        <div className="fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-50">
            <div className="bg-white rounded-2xl shadow-lg p-6 w-full max-w-md space-y-4">
                <h2 className="text-xl font-semibold">
                {roomToEdit ? "Redigera rum" : "Skapa nytt rum"}
                </h2>

                {/* Namn */}
                <div className="space-y-2">
                    <label htmlFor="name" className="block text-sm font-medium text-gray-700">Rum Namn</label>
                    <input
                        id="name"
                        className="w-full border rounded p-2"
                        placeholder="Namn"
                        name="name"
                        value={form.name}
                        onChange={handleChange}
                    />
                </div>

                {/* Beskrivning */}
                <div className="space-y-2">
                    <label htmlFor="description" className="block text-sm font-medium text-gray-700">Beskrivning</label>
                    <textarea
                        id="description"
                        className="w-full border rounded p-2"
                        placeholder="Beskrivning"
                        name="description"
                        value={form.description}
                        onChange={handleChange}
                    />
                </div>

                {/* Våning, Stolar, Luftkvalitet */}
                <div className="grid grid-cols-2 gap-2">
                    <div className="space-y-2">
                        <label htmlFor="floor" className="block text-sm font-medium text-gray-700">Våning</label>
                        <input
                            id="floor"
                            type="number"
                            className="border rounded p-2"
                            placeholder="Våning"
                            name="floor"
                            value={form.floor}
                            onChange={handleChange}
                        />
                    </div>

                    <div className="space-y-2">
                        <label htmlFor="chairs" className="block text-sm font-medium text-gray-700">Stolar</label>
                        <input
                            id="chairs"
                            type="text"
                            className="border rounded p-2"
                            placeholder="Stolar"
                            name="chairs"
                            value={form.chairs}
                            onChange={handleChange}
                        />
                    </div>

                </div>

                {/* Checkboxar: Skärm, Whiteboard, Projektor */}
                <div className="grid grid-cols-2 gap-2 text-sm">
                    <label className="flex items-center space-x-2">
                        <input
                            type="checkbox"
                            name="screen"
                            checked={form.screen}
                            onChange={handleChange}
                        />
                        <span>Skärm</span>
                    </label>
                    <label className="flex items-center space-x-2">
                        <input
                            type="checkbox"
                            name="whiteboard"
                            checked={form.whiteboard}
                            onChange={handleChange}
                        />
                        <span>Whiteboard</span>
                    </label>
                    <label className="flex items-center space-x-2">
                        <input
                            type="checkbox"
                            name="projector"
                            checked={form.projector}
                            onChange={handleChange}
                        />
                        <span>Projektor</span>
                    </label>
                </div>

                {/* Knapp för att skicka formuläret */}
                <div className="flex justify-end space-x-2">
                    <Button variant="outline" onClick={onClose}>Avbryt</Button>
                    <Button onClick={handleSubmit}>{roomToEdit ? "Spara ändringar" : "Skapa"}</Button>
                </div>
            </div>
        </div>
    );
};

export default CreateRoomModal;
