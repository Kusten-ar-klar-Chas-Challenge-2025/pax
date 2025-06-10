import { useState, useEffect } from "react";
import Footer from "./components/Footer";
import RoomCard from "./components/RoomCard";
import CreateRoomModal from "./components/CreateRoomModal";
import Sidebar from "./components/Sidebar";

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

const App: React.FC = () => {
  const [rooms, setRooms] = useState<Room[]>([]);
  const [isModalOpen, setIsModalOpen] = useState(false);
  const [editingRoom, setEditingRoom] = useState<Room | null>(null);

  const API_BASE_URL = "http://localhost:13000";

  // fetch rooms
  useEffect(() => {
    const fetchRooms = async () => {
      try {
        const res = await fetch(`${API_BASE_URL}/rooms`);
        const data = await res.json();
        console.log("Fetched rooms:", data); // Add this
        setRooms(data);
      } catch (error) {
        console.error("Error fetching rooms:", error);
      }
    };

    fetchRooms();
  }, []);

  // Delete rooms frontend
  const handleDeleteRoom = async (id: number) => {
    if (!window.confirm("Är du säker på att du vill ta bort detta rum?"))
      return;
    try {
      const res = await fetch(`${API_BASE_URL}/rooms/${id}`, {
        method: "DELETE",
      });
      if (!res.ok) {
        throw new Error("Kunde inte ta bort rummet");
      }

      setRooms((prevRooms) => prevRooms.filter((room) => room.id !== id));
    } catch (error) {
      console.error("Fel deleting", error);
    }
  };

  // Edit rooms frontend
  const handleEditRoom = (room: Room) => {
    setEditingRoom(room);
    setIsModalOpen(true);
  };

  const handleCreateRoom = async (room: Room) => {
    try {
      const res = await fetch(`${API_BASE_URL}/rooms`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(room),
      });

      if (!res.ok) throw new Error("Kunde inte skapa rummet");

      const newRoom = await res.json();
      setRooms((prev) => [...prev, newRoom]);
    } catch (error) {
      console.error("Fel vid skapande", error);
    }
  };

  const handleUpdateRoom = async (room: Room) => {
    try {
      const res = await fetch(`${API_BASE_URL}/rooms/${room.id}`, {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(room),
      });

      if (!res.ok) throw new Error("Kunde inte uppdatera rummet");

      const updatedRoom = await res.json();

      setRooms((prev) =>
        prev.map((r) => (r.id === updatedRoom.id ? updatedRoom : r))
      );
    } catch (error) {
      console.error("Fel vid uppdatering", error);
    } finally {
      setEditingRoom(null);
    }
  };

  return (
    <div className="min-h-screen flex">
      {/* Sidebar with button trigger */}
      <Sidebar
        onCreateRoomClick={() => {
          setEditingRoom(null); // clear edit state
          setIsModalOpen(true);
        }}
      />

      {/* Main content */}
      <div className="flex flex-col flex-grow bg-gray-300">
        {/* <Header /> */}

        <main className="flex-grow p-6">
          <div className="flex justify-between items-center mb-6">
            <h1 className="text-2xl font-bold">Dashboard</h1>
            <h2 className="text-xl font-semibold">Alla rum</h2>
          </div>

          {rooms.length === 0 ? (
            <p className="text-gray-500">Inga rum ännu. Skapa ett nytt rum!</p>
          ) : (
            <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-6">
              {rooms.map((room) => (
                <RoomCard
                  key={room.id}
                  room={room}
                  onEdit={handleEditRoom}
                  onDelete={handleDeleteRoom}
                />
              ))}
            </div>
          )}
        </main>

        <Footer />
      </div>

      {/* Modal för att skapa rum */}
      <CreateRoomModal
        isOpen={isModalOpen}
        onClose={() => {
          setIsModalOpen(false);
          setEditingRoom(null);
        }}
        onCreate={handleCreateRoom}
        onEdit={handleUpdateRoom}
        roomToEdit={editingRoom || undefined}
      />
    </div>
  );
};

export default App;
