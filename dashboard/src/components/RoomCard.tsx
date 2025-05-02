import { FC } from "react";
import Button from "@/components/Button";

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

type RoomCardProps = {
    room: Room;
    onEdit: (room: Room) => void;
    onDelete: (id: number) => void;
};

const RoomCard: FC<RoomCardProps> = ({ room, onEdit, onDelete }) => {
    return (
        <div className="bg-white p-4 rounded-2xl shadow-md space-y-2">
            <h3 className="text-lg font-semibold">{room.name}</h3>
            <p className="text-sm text-gray-600">{room.description}</p>
            <div className="text-sm text-gray-500">
                <p>Våning: {room.floor}</p>
                <p>Stolar: {room.chairs}</p>
                <p>Whiteboard: {room.whiteboard ? "Ja" : "Nej"}</p>
                <p>Projektor: {room.projector ? "Ja" : "Nej"}</p>
                <p>Skärm: {room.screen ? "Ja" : "Nej"}</p>
                <p>Luftkvalitet: {room.air_quality}</p>
                <p>Status: {room.available ? "Tillgänglig" : "Upptagen"}</p>
            </div>
            <div className="flex justify-end space-x-2">
                <Button variant="outline" onClick={() => onEdit(room)}>
                    Redigera
                </Button>
                <Button variant="destructive" onClick={() => onDelete(room.id)}>
                    Ta bort
                </Button>
            </div>
        </div>
    );
};

export default RoomCard;
