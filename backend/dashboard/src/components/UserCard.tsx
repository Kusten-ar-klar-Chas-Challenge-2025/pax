import {FC} from "react";
import Button from "./Button";
import {User} from "../types/user";

type UserCardProps = {
    user: User;
    onEdit: (user: User) => void;
    onDelete: (id: number) => void;
};

const UserCard: FC<UserCardProps> = ({user, onEdit, onDelete}) => (
    <div className="bg-white p-4 rounded-2xl shadow-md space-y-2">
    <h3 className="text-lg font-semibold">{user.name} {user.surname}</h3>
    <p className="text-sm text-gray-600">{user.email}</p>
    <div className="text-sm text-gray-500">
      <p>Role: {user.role}</p>
    </div>
    <div className="flex justify-end space-x-2">
      <Button variant="outline" onClick={() => onEdit(user)}>Update</Button>
      <Button variant="destructive" onClick={() => onDelete(user.id)}>Delete</Button>
    </div>
  </div>
);

export default UserCard;